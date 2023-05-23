#ifndef _AQUARIUM_H_
#define _AQUARIUM_H_

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <sys/queue.h>
// #include "structures.h"
#include "fish.h"
// #include "view.h"

struct aquarium *create_aquarium(int height, int width);
void deep_copy_aquarium(struct aquarium *dest, struct aquarium *src);
void show_aquarium(struct aquarium *aquarium, FILE *f);
void save_aquarium(struct aquarium *aquarium, char *name);
int free_aquarium(struct aquarium *aquarium);

#endif // _AQUARIUM_H_