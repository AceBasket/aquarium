#ifndef _AQUARIUM_H_
#define _AQUARIUM_H_

#include <stdio.h>
#include <time.h>

#define OK 1
#define NOK 0

enum movement_pattern { RANDOMWAYPOINT };
enum status { STARTED, NOT_STARTED };

struct coordinates {
    int x;
    int y;
};

struct fish {
    const char *name;
    struct coordinates top_left;
    struct coordinates destination;
    time_t time_to_destination;
    int height;
    int width;
    enum movement_pattern movement_pattern;
    enum status status; // started or not started
    struct fish *next;
};

struct aquarium {
    struct fish *fishes; // chained list to fishes
    struct view *views; // chained list to views
    int height;
    int width;
    char **view_names; // list of view names ()
};

struct view {
    const char *name;
    int socket_fd;
    struct coordinates top_left;
    int height;
    int width;
    struct view *next;
};

struct aquarium *create_aquarium(int height, int width);
struct view *get_view(struct aquarium *a, char *name);
struct fish *get_fish_from_name(struct aquarium *a, char *name);
int remove_view(struct aquarium *a, struct view *v);
int remove_fish(struct aquarium *a, struct fish *f);
int add_view(struct aquarium *a, struct view *v);
int add_fish(struct aquarium *a, struct fish *f);
struct fish *create_fish(char *name, struct coordinates top_left, int height, int width, enum movement_pattern mvt);
struct view *create_view(char *name, struct coordinates top_left, int height, int width);
int len_views(struct aquarium *a);
int len_fishes(struct aquarium *a);
void show_aquarium(struct aquarium *a, FILE *f);
void save_aquarium(struct aquarium *a, char *name);
int start_fish(struct aquarium *a, char *name);
struct fish **get_fishes_in_view(struct aquarium *a, struct view *v);
void set_movement(struct aquarium *a, struct fish *f);


#endif // _AQUARIUM_H_