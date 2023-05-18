#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils.h"
#include "prompt_thread.h"
#include "handler_functions/prompt_handler_functions.h"

#define BUFFER_SIZE 256



void *thread_prompt(void *parameters) {
    FILE *log = ((struct thread_prompt_parameters *)parameters)->log;

    signal(SIGPIPE, sigpipe_handler);

    // struct thread_prompt_parameters *params = (struct thread_prompt_parameters *)parameters;
    (void)parameters;

    fprintf(log, "===== thread_prompt() =====\n");
    fflush(log);

    char buffer[BUFFER_SIZE] = {}; // No uninitialized memory
    char char_read;
    int i_buffer;

    pthread_mutex_lock(&terminate_threads_mutex);
    while (terminate_threads == NOK) {
        pthread_mutex_unlock(&terminate_threads_mutex);
        i_buffer = 0;

        // we get the line from the terminal where the user wrote his command
        do {
            char_read = fgetc(stdin);
            if (char_read == EOF) {
                pthread_mutex_lock(&terminate_threads_mutex);
                terminate_threads = OK;
                pthread_mutex_unlock(&terminate_threads_mutex);
                break;
            }
            buffer[i_buffer] = char_read;
            i_buffer++;
        } while (char_read != '\n' && char_read != EOF);

        pthread_mutex_lock(&terminate_threads_mutex);
        if (terminate_threads == OK) {
            pthread_mutex_unlock(&terminate_threads_mutex);
            break;
        }
        pthread_mutex_unlock(&terminate_threads_mutex);

        buffer[i_buffer - 1] = '\0';

        fprintf(log, "Command: '%s' of size %ld\n", buffer, strlen(buffer));
        fflush(log);

        // we parse this line
        struct parse *parser = parse_prompt(buffer);
        enum func function = parser->func_name;
        fprintf(log, "Function to execute: %d\n", function);
        fflush(log);

        switch (function) {
        case LOAD:
            fprintf(log, "Loading aquarium from file %s\n", parser->arguments[0]);
            fflush(log);
            pthread_mutex_lock(&aquarium_mutex);
            load_handler(log, parser, &aquarium);
            pthread_mutex_unlock(&aquarium_mutex);
            break;
        case SHOW:
            fprintf(log, "Showing aquarium\n");
            pthread_mutex_lock(&aquarium_mutex);
            show_handler(log, aquarium);
            pthread_mutex_unlock(&aquarium_mutex);
            break;
        case ADD_VIEW:
            fprintf(log, "Adding view %s to the aquarium\n", parser->arguments[1]);
            pthread_mutex_lock(&aquarium_mutex);
            add_view_handler(log, parser, aquarium);
            pthread_mutex_unlock(&aquarium_mutex);
            break;
        case DEL_VIEW:
            fprintf(log, "Deleting view %s from the aquarium\n", parser->arguments[1]);
            pthread_mutex_lock(&aquarium_mutex);
            del_view_handler(log, parser, aquarium);
            pthread_mutex_unlock(&aquarium_mutex);
            break;
        case SAVE:
            fprintf(log, "Saving the aquarium at %s\n", parser->arguments[0]);
            pthread_mutex_lock(&aquarium_mutex);
            save_handler(log, parser, aquarium);
            pthread_mutex_unlock(&aquarium_mutex);
            break;
        default:
            break;
        }
        free_parser(parser);
        fflush(log);
        pthread_mutex_lock(&terminate_threads_mutex);
    }
    pthread_mutex_unlock(&terminate_threads_mutex);
    free(parameters);
    fprintf(log, "===== thread_prompt() terminated =====\n");
    fflush(log);
    return EXIT_SUCCESS;
}
