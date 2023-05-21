#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <errno.h>
#include <signal.h>

#define OK 1
#define NOK 0
#define MAX_VIEWS 8
#define BUFFER_size 512

#ifndef TESTS
extern volatile int terminate_threads;
extern struct aquarium *aquarium; // global aquarium 
extern pthread_mutex_t aquarium_mutex;
extern pthread_mutex_t views_sockets_mutex;
extern pthread_mutex_t terminate_threads_mutex;
extern pthread_mutex_t views_sockets_mutex;

void sigpipe_handler(int signum);
#endif
void exit_if(int condition, const char *prefix);

#endif // _UTILS_H_