#ifndef _FISH_H_
#define _FISH_H_

#include <stdlib.h>
#include "aquarium.h"
#include "view.h"

enum movement_pattern { RANDOMWAYPOINT };
enum status { STARTED, NOT_STARTED };

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
void free_fishes_array(struct fish **fishes, struct view *view);
struct fish **get_fishes_in_view_and_with_destination_in_view(struct aquarium *aquarium, struct view *view, int started);

#endif // _FISH_H_ 