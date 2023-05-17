#ifndef _IO_THREAD_H_
#define _IO_THREAD_H_
#include <pthread.h>
#include <sys/types.h>
#include "../aquarium/aquarium.h"

struct thread_io_parameters {
    int *views_socket_fd;
    int display_timeout_value;
};

void *thread_timeout(void *parameters);
void *thread_io(void *parameters);

#endif // _IO_THREAD_H_