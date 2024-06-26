#ifndef _ACCEPT_THREAD_H_
#define _ACCEPT_THREAD_H_

#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include "io_thread.h"

struct thread_accept_parameters {
    int nb_views;
    int socket_fd;
    socklen_t view_addr_len;
    struct sockaddr_in view_addr;
    int *views_sockets;
    pthread_t *tid_io;
    pthread_t *tid_timeout;
    int display_timeout_value;
    struct thread_io_parameters *io_parameters;
    FILE *log;
};

/* thread responsible for the connection of clients */
void *thread_accept(void *parameters);

#endif // _ACCEPT_THREAD_H_