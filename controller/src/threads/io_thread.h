#ifndef _IO_THREAD_H_
#define _IO_THREAD_H_

#include <pthread.h>
#include <sys/types.h>
#include "handler_functions/io_handler_functions.h"
#include "../parser/client_inputs_parser.h"


struct thread_io_parameters {
    int *views_socket_fd;
    int display_timeout_value;
    FILE *log;
};

/* thread responsible for customer disconnection */
void *thread_timeout(void *parameters);
/* thread responsible for communication with customers */
void *thread_io(void *parameters);

#endif // _IO_THREAD_H_