#ifndef _AQUARIUM_H_
#define _AQUARIUM_H_

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <sys/queue.h>
#include "fish.h"

/* creates an aquarium */
struct aquarium *create_aquarium(int height, int width);
/* copies an aquarium into another one */
void deep_copy_aquarium(struct aquarium *dest, struct aquarium *src);
/* shows an aquarium load from the file f */
void show_aquarium(struct aquarium *aquarium, FILE *f);
/* saves an aquarium in the path name */
void save_aquarium(struct aquarium *aquarium, char *name);
/* frees an aquarium */
int free_aquarium(struct aquarium *aquarium);

#endif // _AQUARIUM_H_