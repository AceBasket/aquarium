#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <pthread.h>

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
extern enum log_level verbosity_level;

void sigpipe_handler(int signum);

#endif

/* exits the program with the message if the condition is true */
void exit_if(int condition, const char *message);
/* gets current epoch time in milliseconds */
unsigned long long get_time_in_milliseconds();
/* adds a specified number of second to the specified time (which is in milliseconds) */
unsigned long long add_seconds_to_time_in_milliseconds(unsigned long long time, int seconds);
/* returns the time (in seconds) to get the specified time in milliseconds */
time_t get_seconds_to_get_to_time_in_milliseconds(unsigned long long time);
/* prints the message in the log file according to a verbosity level */
void log_message(FILE *log, enum log_level level, const char *message, ...);

#endif // _UTILS_H_