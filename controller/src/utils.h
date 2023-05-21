#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>

#define OK 1
#define NOK 0
#define MAX_VIEWS 8
#define BUFFER_size 512

enum log_level { LOG_FATAL_ERROR, LOG_ERROR, LOG_WARNING, LOG_INFO };

extern enum log_level verbosity_level;
#ifndef TESTS
extern volatile int terminate_threads;
extern struct aquarium *aquarium; // global aquarium 
extern pthread_mutex_t aquarium_mutex;
extern pthread_mutex_t views_sockets_mutex;
extern pthread_mutex_t terminate_threads_mutex;
extern pthread_mutex_t views_sockets_mutex;

void sigpipe_handler(int signum);
#endif
void exit_if(int condition, const char *message);
void log_message(FILE *log, enum log_level level, const char *message, ...);

#endif // _UTILS_H_