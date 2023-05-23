#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <pthread.h>
#include "../parser/file_parser.h"
#include "../threads/accept_thread.h"
#include "../threads/prompt_thread.h"


struct init_server_parameters {
    int nb_views;
    int port_number;
    pthread_t *tid_accept;
    pthread_t *tid_prompt;
    pthread_t *tid_io;
    pthread_t *tid_timeout;
    struct thread_prompt_parameters *prompt_parameters;
    struct thread_accept_parameters *accept_parameters;
    struct thread_io_parameters *io_parameters;
    int *views_sockets_fd;
    FILE *io_log, *accept_log, *prompt_log, *timeout_log;
};

void init_server(struct init_server_parameters *parameters);

#endif // _CONTROLLER_H_