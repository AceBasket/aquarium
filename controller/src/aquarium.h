#ifndef _AQUARIUM_H_
#define _AQUARIUM_H_

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <sys/queue.h>

#include "utils.h"
// #include "fish.h"
// #include "view.h"

#define OK 1
#define NOK 0

struct coordinates {
    int x;
    int y;
};

struct aquarium {
    struct fish *fishes; // chained list to fishes
    struct view *views; // chained list to views
    int height;
    int width;
    char **view_names; // list of view names ()
};

struct aquarium *create_aquarium(int height, int width);
void show_aquarium(struct aquarium *aquarium, FILE *f);
void save_aquarium(struct aquarium *aquarium, char *name);
int free_aquarium(struct aquarium *aquarium);

#endif // _AQUARIUM_H_