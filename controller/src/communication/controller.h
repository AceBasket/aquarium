#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <pthread.h>
#include "../aquarium/aquarium.h"

void init_server(int nb_views, int port_number, struct aquarium **aquarium, pthread_mutex_t *aquarium_mutex, pthread_mutex_t *views_sockets_mutex);

#endif // _CONTROLLER_H_