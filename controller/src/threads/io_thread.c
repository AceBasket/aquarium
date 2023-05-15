#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include "io_thread.h"
#include "../aquarium/fish.h"
#include "../communication/socket_aquarium.h"
#include "handler_functions/io_handler_functions.h"
#include "../utils.h"
#include "../parser/client_inputs_parser.h"

#define BUFFER_SIZE 256
#define MAX_VIEWS 8 



void *thread_io(void *parameters) {
    struct thread_io_parameters *params = (struct thread_io_parameters *)parameters;
    pthread_mutex_t *aquarium_mutex = params->aquarium_mutex;
    struct aquarium **aquarium = params->aquarium;
    int *prompt_thread_terminated = params->prompt_thread_terminated;
    pthread_mutex_t *prompt_thread_terminated_mutex = params->prompt_thread_terminated_mutex;

    FILE *log = fopen("log_io.log", "w");
    fprintf(log, "===== thread_io() =====\n");
    fflush(log);

    pthread_mutex_lock(aquarium_mutex);
    pthread_mutex_lock(prompt_thread_terminated_mutex);
    while (*aquarium == NULL && *prompt_thread_terminated == NOK) {
        pthread_mutex_unlock(prompt_thread_terminated_mutex);
        pthread_mutex_unlock(aquarium_mutex);
        fprintf(log, "Waiting for aquarium to be initialized\n");
        fflush(log);
        sleep(1);

        pthread_mutex_lock(aquarium_mutex);
        pthread_mutex_lock(prompt_thread_terminated_mutex);
    }
    pthread_mutex_unlock(prompt_thread_terminated_mutex);
    pthread_mutex_unlock(aquarium_mutex);

    // For communication with views
    fd_set read_fds;
    int *views_socket_fd = (int *)params->views_socket_fd;
    int recv_bytes;
    char buffer[BUFFER_SIZE];

    // If get_fishes_continuously is called
    pthread_t handle_fishes_continuously_thread;

    // Wait for views to be initialized
    pthread_mutex_lock(prompt_thread_terminated_mutex);
    while (views_socket_fd[0] == -1 && *prompt_thread_terminated == NOK) {
        pthread_mutex_unlock(prompt_thread_terminated_mutex);
        fprintf(log, "Waiting for views to be initialized\n");
        fflush(log);
        sleep(1);
        pthread_mutex_lock(prompt_thread_terminated_mutex);
    }
    pthread_mutex_unlock(prompt_thread_terminated_mutex);

    pthread_mutex_lock(prompt_thread_terminated_mutex);
    while (*prompt_thread_terminated == NOK) {
        pthread_mutex_unlock(prompt_thread_terminated_mutex);
        FD_ZERO(&read_fds);
        int max_fd = 0;

        for (int num_view = 0; num_view < MAX_VIEWS; num_view++) {
            FD_SET(views_socket_fd[num_view], &read_fds);
            // for select, we need to know the highest file descriptor number
            if (max_fd < views_socket_fd[num_view]) {
                max_fd = views_socket_fd[num_view];
            }
        }

        // Wait indefinitely for an activity on one of the sockets
        fprintf(log, "Waiting for activity on one of the sockets\n");
        fflush(log);
        exit_if(select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1, "ERROR on select");
        for (int num_view = 0; num_view < MAX_VIEWS; num_view++) {
            if (FD_ISSET(views_socket_fd[num_view], &read_fds)) {
                // we have data on the num_view socket
                fprintf(log, "Received data from view %d\n", num_view);
                fflush(log);

                // read data until we get a \n
                int total_recv_bytes = 0; // later on, if we want to keep listening until the client sends a \n
                while (1) {

                    // If received Ctrl+D on prompt 
                    pthread_mutex_lock(prompt_thread_terminated_mutex);
                    if (*prompt_thread_terminated == NOK) {
                        pthread_mutex_unlock(prompt_thread_terminated_mutex);
                        fprintf(log, "===== thread_io() terminated (while reading from socket) =====\n");
                        fflush(log);
                        fclose(log);
                        // free memory
                        return NULL;
                    }
                    pthread_mutex_unlock(prompt_thread_terminated_mutex);

                    char char_read;
                    recv_bytes = recv(views_socket_fd[num_view], &char_read, 1, 0);
                    exit_if(recv_bytes == -1, "ERROR on recv");
                    if (recv_bytes == 0) {
                        fprintf(log, "Client closed connection\n");
                        fflush(log);
                        break;
                    } else {
                        if (char_read != '\r') {
                            buffer[total_recv_bytes++] = char_read;
                        }
                        if (char_read == '\n') {
                            buffer[total_recv_bytes - 1] = '\0';
                            // we have a full line
                            fprintf(log, "Received %d bytes from view %d: %s\n", total_recv_bytes, num_view, buffer);
                            fflush(log);
                            break;
                        }
                    }
                }

                // fprintf(log, "before parse_clients\n");
                // fflush(log);
                struct parse *parser = parse_clients(buffer);
                enum func function_called = parser->func_name;
                // fprintf(log, "after parse_clients\n");
                // fprintf(log, "function_called: %d\n", function_called);
                // fflush(log);
                switch (function_called) {
                case HELLO:
                    fprintf(log, "Hello from view %d\n", num_view);
                    pthread_mutex_lock(aquarium_mutex);
                    hello_handler(log, parser, views_socket_fd[num_view], *aquarium);
                    pthread_mutex_unlock(aquarium_mutex);
                    break;
                case GETFISHES:
                    fprintf(log, "Get fishes from view %d\n", num_view);
                    pthread_mutex_lock(aquarium_mutex);
                    get_fishes_handler(log, parser, views_socket_fd[num_view], *aquarium);
                    pthread_mutex_unlock(aquarium_mutex);
                    break;
                case GFCONTINUOUSLY:
                    fprintf(log, "Get fishes continuously from view %d\n", num_view);
                    pthread_mutex_lock(aquarium_mutex);
                    get_fishes_continuously_handler(log, parser, views_socket_fd[num_view], *aquarium, aquarium_mutex, &handle_fishes_continuously_thread);
                    pthread_mutex_unlock(aquarium_mutex);
                    break;
                case LS:
                    fprintf(log, "List fishes from view %d\n", num_view);
                    pthread_mutex_lock(aquarium_mutex);
                    ls_handler(log, parser, views_socket_fd[num_view], *aquarium);
                    pthread_mutex_unlock(aquarium_mutex);
                    break;
                case PING:
                    fprintf(log, "Ping from view %d\n", num_view);
                    pthread_mutex_lock(aquarium_mutex);
                    ping_handler(log, parser, views_socket_fd[num_view], *aquarium);
                    pthread_mutex_unlock(aquarium_mutex);
                    break;
                case ADDFISH:
                    fprintf(log, "Add fish from view %d\n", num_view);
                    pthread_mutex_lock(aquarium_mutex);
                    add_fish_handler(log, parser, views_socket_fd[num_view], *aquarium);
                    pthread_mutex_unlock(aquarium_mutex);
                    break;
                case DELFISH:
                    fprintf(log, "Delete fish from view %d\n", num_view);
                    pthread_mutex_lock(aquarium_mutex);
                    del_fish_handler(log, parser, views_socket_fd[num_view], *aquarium);
                    pthread_mutex_unlock(aquarium_mutex);
                    break;
                case STARTFISH:
                    fprintf(log, "Start fish (%s) from view %d\n", parser->arguments[0], num_view);
                    pthread_mutex_lock(aquarium_mutex);
                    start_fish_handler(log, parser, views_socket_fd[num_view], *aquarium);
                    pthread_mutex_unlock(aquarium_mutex);
                    break;
                case LOG:
                    fprintf(log, "LOGOUT out from view %d\n", num_view);
                    pthread_mutex_lock(aquarium_mutex);
                    log_out_handler(log, parser, views_socket_fd[num_view], *aquarium);
                    pthread_mutex_unlock(aquarium_mutex);
                    break;
                // case STATUS:
                //     fprintf(log, "Status from view %d\n", num_view);
                //     status_handler(log, parser, views_socket_fd[num_view], *aquarium);
                //     break;
                case UNKNOWN:
                    fprintf(log, "Unknown command from view %d\n", num_view);
                    dprintf(views_socket_fd[num_view], "NOK: Unknown command\n");
                    break;
                default:
                    dprintf(views_socket_fd[num_view], "%s", parser->status);
                    break;
                }
                free_parser(parser);
                fflush(log);
            }
        }
        pthread_mutex_lock(prompt_thread_terminated_mutex);
    }
    pthread_mutex_unlock(prompt_thread_terminated_mutex);
    pthread_join(handle_fishes_continuously_thread, NULL);
    fprintf(log, "===== thread_io() terminated =====\n");
    fflush(log);
    fclose(log);
    // free memory
    return EXIT_SUCCESS;
}