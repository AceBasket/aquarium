#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <errno.h>
#include <signal.h>

#define OK 1
#define NOK 0
#define MAX_VIEWS 8
#define BUFFER_size 512

extern volatile int terminate_threads;

void exit_if(int condition, const char *prefix);
void sigpipe_handler(int signum);

#endif // _UTILS_H_