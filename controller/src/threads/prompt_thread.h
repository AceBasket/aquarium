#ifndef _PROMPT_THREAD_H_
#define _PROMPT_THREAD_H_

#include <pthread.h>
#include "handler_functions/prompt_handler_functions.h"

struct thread_prompt_parameters {
    FILE *log;
};

/* thread responsible for the prompter of the controller */
void *thread_prompt(void *parameters);

#endif // _PROMPT_THREAD_H_