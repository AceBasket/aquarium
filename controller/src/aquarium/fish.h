#ifndef _FISH_H_
#define _FISH_H_

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
int add_specific_destination(struct fish *fish, struct fish_destination *destination);
struct fish **get_fishes_in_view(struct aquarium *aquarium, struct view *view, int started);
struct fish **get_fishes_with_destination_in_view(struct aquarium *aquarium, struct view *view, int started);
int coordinates_are_in_view(struct coordinates *c, struct view *view);
int len_fishes(struct aquarium *aquarium);
int start_fish(struct aquarium *aquarium, char *name);
float distance(struct coordinates destination, struct coordinates origin);
int add_movement(struct aquarium *aquarium, struct fish *fish);
int update_fish_coordinates(struct fish *fish);
int remove_finished_movements(struct fish *fish);
int len_movements_queue(struct fish *fish);
void free_fishes_array(struct fish **fishes, struct view *view);
int add_intermediate_movements(struct aquarium *aquarium, struct fish *fish, struct fish_destination *origin, struct fish_destination *destination);
struct coordinates *get_instersections_btw_trajectory_and_views(struct view **views, struct coordinates *p1, struct coordinates *p2);
void debug_destinations_queue(FILE *log, struct fish *fish);

#endif // _FISH_H_ 