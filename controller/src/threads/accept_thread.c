#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "accept_thread.h"


void *thread_accept(void *parameters) {
    // Initialization of the parameters
    struct thread_accept_parameters *params = parameters;
    int *views_sockets = params->views_sockets;
    struct sockaddr_in view_addr = params->view_addr;
    socklen_t view_addr_len = params->view_addr_len;
    int main_socket_fd = params->socket_fd;
    int nb_views = params->nb_views;
    pthread_t *tid_io = params->tid_io;
    pthread_t *tid_timeout = params->tid_timeout;
    struct thread_io_parameters *io_parameters = params->io_parameters;
    FILE *log = params->log;
    log_message(log, LOG_INFO, "===== thread_accept() =====");

    int new_socket_fd;

    if (signal(SIGPIPE, sigpipe_handler) == SIG_ERR) {
        log_message(log, LOG_ERROR, "The signal handler could not be changed");
    }

    // Initialization of all views_socket[] to -1 so not checked
    memset(views_sockets, -1, MAX_VIEWS*sizeof(int));

    io_parameters->views_socket_fd = views_sockets;
    io_parameters->display_timeout_value = params->display_timeout_value;

    if (pthread_create(tid_io, NULL, thread_io, io_parameters) != 0) {
        log_message(log, LOG_FATAL_ERROR, "On io thread creation");
    } 
    if (pthread_create(tid_timeout, NULL, thread_timeout, io_parameters) != 0) {
        log_message(log, LOG_FATAL_ERROR, "On timeout thread creation");
    } 

    pthread_mutex_lock(&terminate_threads_mutex);
    while (terminate_threads == NOK) {
        pthread_mutex_unlock(&terminate_threads_mutex);
        log_message(log, LOG_INFO, "Waiting for a new connection...");

        view_addr_len = sizeof(view_addr);
        new_socket_fd = accept(main_socket_fd, (struct sockaddr *)&view_addr, &view_addr_len);
        if (new_socket_fd < 0) {
            log_message(log, LOG_FATAL_ERROR, "ERROR on accept");
        } 
        log_message(log, LOG_INFO, "Welcome");

        // Adding the new socket to the array of sockets
        for (int k = 0; k < nb_views; k++) {
            // If the position is empty
            if (views_sockets[k] == -1) {
                views_sockets[k] = new_socket_fd;
                log_message(log, LOG_INFO, "Adding to list of sockets as %d", k);
                break;
            }
        }
        pthread_mutex_lock(&terminate_threads_mutex);
    }
    pthread_mutex_unlock(&terminate_threads_mutex);

    free(((struct thread_accept_parameters *)parameters)->views_sockets);
    free(((struct thread_accept_parameters *)parameters)->views_sockets);
    free(parameters);
    log_message(log, LOG_INFO, "===== thread_accept() terminated =====");
    return EXIT_SUCCESS;
}