#ifndef _ACCEPT_THREAD_H_
#define _ACCEPT_THREAD_H_
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>

struct thread_accept_parameters {
    int nb_views;
    int socket_fd;
    socklen_t view_addr_len;
    struct sockaddr_in view_addr;
    int *views_sockets;
    pthread_mutex_t *views_sockets_mutex;
    pthread_mutex_t *aquarium_mutex;
    struct aquarium **aquarium;
};

void *thread_accept(void *parameters);

#endif // _ACCEPT_THREAD_H_