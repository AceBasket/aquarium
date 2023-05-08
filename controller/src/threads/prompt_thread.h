#ifndef _PROMPT_THREAD_H_
#define _PROMPT_THREAD_H_
#include <pthread.h>
#include "../aquarium/aquarium.h"


struct thread_prompt_parameters {
    struct aquarium **aquarium;
    pthread_mutex_t *aquarium_mutex;
};


void *thread_prompt(void *parameters);
#endif // _PROMPT_THREAD_H_