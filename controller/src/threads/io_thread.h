#ifndef _IO_THREAD_H_
#define _IO_THREAD_H_
#include <pthread.h>
#include <sys/types.h>
#include "../aquarium/aquarium.h"

struct thread_io_parameters {
    int *views_socket_fd;
    struct aquarium **aquarium;
    pthread_mutex_t *aquarium_mutex;
    pthread_mutex_t *views_sockets_mutex;
    pthread_mutex_t *terminate_threads_mutex;
};

void *thread_io(void *parameters);

#endif // _IO_THREAD_H_