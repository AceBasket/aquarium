#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils.h"
#include "prompt_thread.h"
#include "handler_functions/prompt_handler_functions.h"

#define BUFFER_SIZE 256


void *thread_prompt(void *parameters) {
    FILE *log = ((struct thread_prompt_parameters *)parameters)->log;
    log_message(log, LOG_INFO, "===== thread_prompt() =====");

    if (signal(SIGPIPE, sigpipe_handler) == SIG_ERR) {
        log_message(log, LOG_ERROR, "The signal handler could not be changed");
    }

    // struct thread_prompt_parameters *params = (struct thread_prompt_parameters *)parameters;
    (void)parameters;


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
                // log_message(log, LOG_WARNING, "End of line");
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

        log_message(log, LOG_INFO, "Command: '%s' of size %ld", buffer, strlen(buffer));

        // we parse this line
        struct parse *parser = parse_prompt(buffer);
        enum func function = parser->func_name;
        //log_message(log, LOG_INFO, "Function to execute: %d", function);
        switch (function) {
        case LOAD:
            log_message(log, LOG_INFO, "Loading aquarium from file %s", parser->arguments[0]);
            pthread_mutex_lock(&aquarium_mutex);
            load_handler(log, parser, &aquarium);
            pthread_mutex_unlock(&aquarium_mutex);
            break;
        case SHOW:
            log_message(log, LOG_INFO, "Showing aquarium");
            show_handler(log, aquarium);
            pthread_mutex_unlock(&aquarium_mutex);
            break;
        case ADD_VIEW:
            log_message(log, LOG_INFO, "Adding view %s to the aquarium", parser->arguments[1]);
            pthread_mutex_lock(&aquarium_mutex);
            add_view_handler(log, parser, aquarium);
            pthread_mutex_unlock(&aquarium_mutex);
            break;
        case DEL_VIEW:
            log_message(log, LOG_INFO, "Deleting view %s from the aquarium", parser->arguments[0]);
            pthread_mutex_lock(&aquarium_mutex);
            del_view_handler(log, parser, aquarium);
            pthread_mutex_unlock(&aquarium_mutex);
            break;
        case SAVE:
            log_message(log, LOG_INFO, "Saving the aquarium at %s", parser->arguments[0]);
            pthread_mutex_lock(&aquarium_mutex);
            save_handler(log, parser, aquarium);
            pthread_mutex_unlock(&aquarium_mutex);
            break;
        default:
            break;
        }
        free_parser(parser);
        pthread_mutex_lock(&terminate_threads_mutex);
    }
    pthread_mutex_unlock(&terminate_threads_mutex);
    log_message(log, LOG_INFO, "===== thread_prompt() terminated =====");
    return EXIT_SUCCESS;
}
