#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "accept_thread.h"
#include "../utils.h"
#include "io_thread.h"

void *thread_accept(void *parameters) {
    FILE *log = fopen("log_accept.log", "w");

    signal(SIGPIPE, sigpipe_handler);

    // Initialization of the parameters
    struct thread_accept_parameters *params = parameters;
    int *views_sockets = params->views_sockets;
    struct sockaddr_in view_addr = params->view_addr;
    socklen_t view_addr_len = params->view_addr_len;
    int main_socket_fd = params->socket_fd;
    int nb_views = params->nb_views;
    pthread_mutex_t *terminate_threads_mutex = params->terminate_threads_mutex;
    pthread_t *tid_io = params->tid_io;

    int new_socket_fd;

    fprintf(log, "===== thread_accept() =====\n");
    fflush(log);

    // Initialization of all views_socket[] to 0 so not checked
    memset(views_sockets, -1, sizeof(views_sockets) * MAX_VIEWS);

    struct thread_io_parameters *io_parameters = malloc(sizeof(struct thread_io_parameters));
    io_parameters->views_socket_fd = views_sockets;
    io_parameters->aquarium_mutex = params->aquarium_mutex;
    io_parameters->aquarium = params->aquarium;
    io_parameters->views_sockets_mutex = params->views_sockets_mutex;
    io_parameters->terminate_threads_mutex = terminate_threads_mutex;

    exit_if(pthread_create(tid_io, NULL, thread_io, io_parameters) < 0, "ERROR on thread creation");
        // exit_if(pthread_detach(tid_io) != 0, "ERROR in thread detachment");
    pthread_mutex_lock(terminate_threads_mutex);
    while (terminate_threads == NOK) {
        pthread_mutex_unlock(terminate_threads_mutex);
        fprintf(log, "Waiting for a new connection...\n");
        fflush(log);

        view_addr_len = sizeof(view_addr);
        new_socket_fd = accept(main_socket_fd, (struct sockaddr *)&view_addr, &view_addr_len);
        exit_if(new_socket_fd < 0, "ERROR on accept");
        fprintf(log, "Welcome\n");
        // Adding the new socket to the array of sockets
        for (int k = 0; k < nb_views; k++) {
            // If the position is empty
            if (views_sockets[k] == -1) {
                views_sockets[k] = new_socket_fd;
                fprintf(log, "Adding to list of sockets as %d\n", k);
                break;
            }
        }
        fflush(log);
        pthread_mutex_lock(terminate_threads_mutex);
    }
    pthread_mutex_unlock(terminate_threads_mutex);
    free(io_parameters);
    fprintf(log, "===== thread_accept() terminated =====\n");
    fflush(log);
    fclose(log);
    return EXIT_SUCCESS;
}