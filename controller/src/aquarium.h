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
    char *name;
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
    char *name;
    int socket_fd;
    struct coordinates top_left;
    int height;
    int width;
    struct view *next;
};

struct aquarium *create_aquarium(int height, int width);
struct view *get_view(struct aquarium *aquarium, char *name);
struct fish *get_fish_from_name(struct aquarium *aquarium, char *name);
int remove_view(struct aquarium *aquarium, struct view *view);
int remove_fish(struct aquarium *aquarium, struct fish *fish);
int add_view(struct aquarium *aquarium, struct view *view);
int add_fish(struct aquarium *aquarium, struct fish *fish);
struct fish *create_fish(char *name, struct coordinates top_left, int height, int width, enum movement_pattern mvt);
struct view *create_view(char *name, struct coordinates top_left, int height, int width);
int len_views(struct aquarium *aquarium);
int len_fishes(struct aquarium *aquarium);
void show_aquarium(struct aquarium *aquarium, FILE *f);
void save_aquarium(struct aquarium *aquarium, char *name);
int start_fish(struct aquarium *aquarium, char *name);
struct fish **get_fishes_in_view(struct aquarium *aquarium, struct view *view, int started);
void set_movement(struct aquarium *aquarium, struct fish *fish);
int free_aquarium(struct aquarium *aquarium);

#endif // _AQUARIUM_H_