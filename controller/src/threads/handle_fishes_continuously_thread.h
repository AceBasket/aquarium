#ifndef _HANDLE_FISHES_CONTINUOUSLY_THREAD_H_
#define _HANDLE_FISHES_CONTINUOUSLY_THREAD_H_

#include <pthread.h>
#include "../aquarium/aquarium.h"

struct handle_fishes_continuously_parameters {
    struct aquarium *aquarium;
    int socket_fd;
    pthread_mutex_t *aquarium_mutex;
    int *prompt_thread_terminated;
    pthread_mutex_t *prompt_thread_terminated_mutex;
};

void *get_fishes_continuously(void *parameters);

#endif // _HANDLE_FISHES_CONTINUOUSLY_THREAD_H_