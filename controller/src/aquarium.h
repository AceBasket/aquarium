#ifndef _AQUARIUM_H_
#define _AQUARIUM_H_

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <sys/queue.h>


#define OK 1
#define NOK 0

enum movement_pattern { RANDOMWAYPOINT };
enum status { STARTED, NOT_STARTED };

struct coordinates {
    int x;
    int y;
};

typedef STAILQ_HEAD(tailq, fish_destination) tailq_t;

struct fish_destination {
    struct coordinates destination_coordinates;
    time_t time_at_destination;
    STAILQ_ENTRY(fish_destination) next;
};

struct fish {
    char *name;
    struct coordinates top_left;
    tailq_t destinations_queue;
    int speed;
    int height;
    int width;
    enum movement_pattern movement_pattern;
    enum status status; // started or not started
    struct fish *next;
};

struct view {
    char *name;
    int socket_fd;
    struct coordinates top_left;
    int height;
    int width;
    struct view *next;
};

struct aquarium {
    struct fish *fishes; // chained list to fishes
    struct view *views; // chained list to views
    int height;
    int width;
    char **view_names; // list of view names ()
};

// ===== FISH =====
struct fish *create_fish(char *name, struct coordinates top_left, int height, int width, enum movement_pattern mvt);
int add_fish(struct aquarium *aquarium, struct fish *fish);
int remove_fish(struct aquarium *aquarium, struct fish *fish);
struct fish *get_fish_from_name(struct aquarium *aquarium, char *name);
struct fish *get_fishes(struct aquarium *aquarium);
struct fish **get_fishes_in_view(struct aquarium *aquarium, struct view *view, int started);
int len_fishes(struct aquarium *aquarium);
int start_fish(struct aquarium *aquarium, char *name);
void add_movement(struct aquarium *aquarium, struct fish *fish);
void remove_finished_movements(struct fish *fish);
int len_movements_queue(struct fish *fish);

// ===== VIEW =====
struct view *create_view(char *name, struct coordinates top_left, int height, int width);
int add_view(struct aquarium *aquarium, struct view *view);
int remove_view(struct aquarium *aquarium, struct view *view);
struct view *get_view(struct aquarium *aquarium, char *name);
int len_views(struct aquarium *aquarium);

// ===== AQUARIUM =====
struct aquarium *create_aquarium(int height, int width);
void show_aquarium(struct aquarium *aquarium, FILE *f);
void save_aquarium(struct aquarium *aquarium, char *name);
int free_aquarium(struct aquarium *aquarium);

#endif // _AQUARIUM_H_