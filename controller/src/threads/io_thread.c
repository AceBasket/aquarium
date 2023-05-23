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

void dprintf_verif(char *m, int fd, FILE *log) {
    int len_message = strlen(m);
    char message[len_message + 1];
    sprintf(message, "%s", m);
    int nb = len_message;
    int offset = 0;
    do {
        const char *currentMessage = message + offset;
        int nb2 = dprintf(fd, "%s", currentMessage);
        if (nb2 < 0) {
            log_message(log, LOG_ERROR, "Could not write on the socket %d", fd);
        }
        offset += nb - offset;
    } while (offset < nb);
}

void *thread_timeout(void *parameters) {
    struct thread_io_parameters *params = (struct thread_io_parameters *)parameters;
    int *views_socket_fd = params->views_socket_fd;
    int timeout = params->display_timeout_value;

    FILE *log = fopen("log_timeout.log", "w");
    log_message(log, LOG_INFO, "===== thread_timeout() =====");

    pthread_mutex_lock(&terminate_threads_mutex);
    while (terminate_threads == NOK) {
        pthread_mutex_unlock(&terminate_threads_mutex);
        for (int num_view = 0; num_view < MAX_VIEWS; num_view++) {

            pthread_mutex_lock(&views_sockets_mutex);
            // Verify that the socket is active
            if (views_socket_fd[num_view] != -1) {
                pthread_mutex_unlock(&views_sockets_mutex);
                time_t current_time = time(NULL);
                if (current_time == -1) {
                    log_message(log, LOG_ERROR, "On time()");
                }

                struct view *view = NULL;
                // Verify that the view is initialized
                while (view == NULL) {
                    log_message(log, LOG_INFO, "View %d is not initialized yet", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    pthread_mutex_lock(&views_sockets_mutex);
                    view = get_view_from_socket(aquarium, views_socket_fd[num_view]);
                    pthread_mutex_unlock(&views_sockets_mutex);
                    pthread_mutex_unlock(&aquarium_mutex);
                    if (usleep(300000) != 0) {
                        log_message(log, LOG_ERROR, "Usleep() interrupted");
                    }
                }

                // Disconnects the view when it has been inactive for too long
                log_message(log, LOG_INFO, "Time left before timeout for view %d: %d", num_view, timeout - (current_time - view->time_last_ping));
                if (view != NULL && current_time - view->time_last_ping >= timeout) {
                    log_message(log, LOG_INFO, "View %d disconnected", num_view);
                    pthread_mutex_lock(&views_sockets_mutex);

                    // if (dprintf(views_socket_fd[num_view], "bye\n") < 0) {
                    //     log_message(log, LOG_ERROR, "Could not write on the socket %d", views_socket_fd[num_view]);
                    // }
                    dprintf_verif("bye\n", views_socket_fd[num_view], log);

                    if (close(views_socket_fd[num_view]) != 0) {
                        log_message(log, LOG_ERROR, "The socket %d could not be closed", views_socket_fd[num_view]);
                    }
                    views_socket_fd[num_view] = -1;
                    view->socket_fd = -1;
                    pthread_mutex_unlock(&views_sockets_mutex);
                    break;
                }
                pthread_mutex_lock(&views_sockets_mutex);
            }
            pthread_mutex_unlock(&views_sockets_mutex);

        }
        if (sleep(1) != 0) {
            log_message(log, LOG_ERROR, "Sleep() interrupted");
        }
        pthread_mutex_lock(&terminate_threads_mutex);
    }
    pthread_mutex_unlock(&terminate_threads_mutex);

    log_message(log, LOG_INFO, "===== thread_timeout() terminated =====");
    fclose(log);
    return NULL;
}



void *thread_io(void *parameters) {
    struct thread_io_parameters *params = (struct thread_io_parameters *)parameters;
    FILE *log = params->log;
    log_message(log, LOG_INFO, "===== thread_io() =====");

    if (signal(SIGPIPE, sigpipe_handler) == SIG_ERR) {
        log_message(log, LOG_ERROR, "The signal handler could not be changed");
    }

    pthread_mutex_lock(&aquarium_mutex);
    pthread_mutex_lock(&terminate_threads_mutex);
    while (aquarium == NULL && terminate_threads == NOK) {
        pthread_mutex_unlock(&terminate_threads_mutex);
        pthread_mutex_unlock(&aquarium_mutex);
        log_message(log, LOG_INFO, "Waiting for aquarium to be initialized");
        if (sleep(1) != 0) {
            log_message(log, LOG_ERROR, "Sleep() interrupted");
        }
        pthread_mutex_lock(&aquarium_mutex);
        pthread_mutex_lock(&terminate_threads_mutex);
    }
    pthread_mutex_unlock(&terminate_threads_mutex);
    pthread_mutex_unlock(&aquarium_mutex);

    // For communication with views
    fd_set read_fds;
    int *views_socket_fd = params->views_socket_fd;
    int recv_bytes;
    char buffer[BUFFER_SIZE] = {}; // No uninitialized memory

    // If get_fishes_continuously is called
    pthread_t handle_fishes_continuously_thread = 0; // To not have a non-initialized value

    // Wait for views to be initialized
    pthread_mutex_lock(&terminate_threads_mutex);
    while (views_socket_fd[0] == -1 && terminate_threads == NOK) {
        pthread_mutex_unlock(&terminate_threads_mutex);
        log_message(log, LOG_INFO, "Waiting for views to be initialized");
        if (sleep(1) != 0) {
            log_message(log, LOG_ERROR, "Sleep() interrupted");
        }
        pthread_mutex_lock(&terminate_threads_mutex);
    }
    pthread_mutex_unlock(&terminate_threads_mutex);

    pthread_mutex_lock(&terminate_threads_mutex);
    while (terminate_threads == NOK) {
        pthread_mutex_unlock(&terminate_threads_mutex);
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
        log_message(log, LOG_INFO, "Waiting for activity on one of the sockets");
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            log_message(log, LOG_FATAL_ERROR, "On select()");
        }
        for (int num_view = 0; num_view < MAX_VIEWS; num_view++) {
            if (FD_ISSET(views_socket_fd[num_view], &read_fds)) {
                // we have data on the num_view socket
                log_message(log, LOG_INFO, "Received data from view %d", num_view);

                struct view *view = get_view_from_socket(aquarium, views_socket_fd[num_view]);

                // read data until we get a \n
                int total_recv_bytes = 0; // later on, if we want to keep listening until the client sends a \n
                while (1) {

                    // If received Ctrl+D on prompt 
                    pthread_mutex_lock(&terminate_threads_mutex);
                    if (terminate_threads == OK) {
                        pthread_mutex_unlock(&terminate_threads_mutex);
                        log_message(log, LOG_INFO, "===== thread_io() terminated (while reading from socket) =====");
                        return NULL;
                    }
                    pthread_mutex_unlock(&terminate_threads_mutex);

                    char char_read;
                    recv_bytes = recv(views_socket_fd[num_view], &char_read, 1, 0);
                    if (recv_bytes == -1) {
                        log_message(log, LOG_ERROR, "On recv()");
                    }
                    if (recv_bytes == 0) {
                        log_message(log, LOG_INFO, "Client closed connection");
                        break;
                    } else {
                        if (char_read != '\r') {
                            buffer[total_recv_bytes++] = char_read;
                        }
                        if (char_read == '\n') {
                            buffer[total_recv_bytes - 1] = '\0';
                            // we have a full line
                            log_message(log, LOG_INFO, "Received %d bytes from view %d: %s", total_recv_bytes, num_view, buffer);
                            break;
                        }
                    }
                }

                struct parse *parser = parse_clients(buffer);
                enum func function_called = parser->func_name;
                switch (function_called) {
                case HELLO:
                    log_message(log, LOG_INFO, "Hello from view %d", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    hello_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case GET_FISHES:
                    log_message(log, LOG_INFO, "Get fishes from view %d", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    get_fishes_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case GET_FISHES_CONTINUOUSLY:
                    log_message(log, LOG_INFO, "Get fishes continuously from view %d", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    get_fishes_continuously_handler(log, parser, views_socket_fd[num_view], &handle_fishes_continuously_thread);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case LS:
                    log_message(log, LOG_INFO, "List fishes from view %d", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    ls_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case PING:
                    log_message(log, LOG_INFO, "Ping from view %d", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    ping_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case ADD_FISH:
                    log_message(log, LOG_INFO, "Add fish from view %d", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    add_fish_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case DEL_FISH:
                    log_message(log, LOG_INFO, "Delete fish from view %d", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    del_fish_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case START_FISH:
                    log_message(log, LOG_INFO, "Start fish (%s) from view %d", parser->arguments[0], num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    start_fish_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case LOG:
                    log_message(log, LOG_INFO, "LOGOUT out from view %d", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    log_out_handler(log, parser, &views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                // case STATUS:
                //     fprintf(log, "Status from view %d\n", num_view);
                //     status_handler(log, parser, views_socket_fd[num_view], *aquarium);
                //     break;
                case UNKNOWN:
                    log_message(log, LOG_INFO, "Unknown command from view %d", num_view);
                    // if (dprintf(views_socket_fd[num_view], "NOK: Unknown command\n") < 0) {
                    //     log_message(log, LOG_ERROR, "Could not write on the socket %d", views_socket_fd[num_view]);
                    // }
                    dprintf_verif("NOK: Unknown command\n", views_socket_fd[num_view], log);
                    break;
                default:
                    // if (dprintf(views_socket_fd[num_view], "%s", parser->status) < 0) {
                    //     log_message(log, LOG_ERROR, "Could not write on the socket %d", views_socket_fd[num_view]);
                    // }
                    dprintf_verif(parser->status, views_socket_fd[num_view], log);
                    break;
                }
                if (view != NULL) {
                    pthread_mutex_lock(&views_sockets_mutex);
                    view->time_last_ping = time(NULL);
                    if (view->time_last_ping == -1) {
                        log_message(log, LOG_ERROR, "On time()");
                    }
                    pthread_mutex_unlock(&views_sockets_mutex);
                }
                free_parser(parser);
            }
        }
        pthread_mutex_lock(&terminate_threads_mutex);
    }
    pthread_mutex_unlock(&terminate_threads_mutex);

    if (pthread_join(handle_fishes_continuously_thread, NULL) != 0) {
        log_message(log, LOG_ERROR, "The handle fishes continuously thread could not be joined");
    }
    free(parameters);
    log_message(log, LOG_INFO, "===== thread_io() terminated =====");

    fclose(log);

    return EXIT_SUCCESS;
}