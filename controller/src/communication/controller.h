#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <pthread.h>
#include "../aquarium/aquarium.h"

struct init_server_parameters {
    int nb_views;
    int port_number;
    struct aquarium **aquarium;
    pthread_mutex_t *aquarium_mutex;
    pthread_mutex_t *views_sockets_mutex;
    pthread_mutex_t *terminate_threads_mutex;
    pthread_t *tid_accept;
    pthread_t *tid_prompt;
    pthread_t *tid_io;
};

void init_server(struct init_server_parameters *parameters);

#endif // _CONTROLLER_H_