#ifndef _HANDLE_FISHES_CONTINUOUSLY_THREAD_H_
#define _HANDLE_FISHES_CONTINUOUSLY_THREAD_H_

#include <pthread.h>
#include "handler_functions/io_handler_functions.h"
#include "handler_functions/list_fishes.h"

struct handle_fishes_continuously_parameters {
    int socket_fd;
};

void *get_fishes_continuously(void *parameters);

#endif // _HANDLE_FISHES_CONTINUOUSLY_THREAD_H_