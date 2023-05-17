#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <pthread.h>
#include "../aquarium/aquarium.h"

struct init_server_parameters {
    int nb_views;
    int port_number;
    pthread_t *tid_accept;
    pthread_t *tid_prompt;
    pthread_t *tid_io;
    pthread_t *tid_timeout;
};

void init_server(struct init_server_parameters *parameters);

#endif // _CONTROLLER_H_