#ifndef _PROMPT_THREAD_H_
#define _PROMPT_THREAD_H_
#include <pthread.h>
#include "../aquarium/aquarium.h"


struct thread_prompt_parameters {};


void *thread_prompt(void *parameters);
#endif // _PROMPT_THREAD_H_