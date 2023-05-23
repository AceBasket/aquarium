#ifndef _FISH_H_
#define _FISH_H_

#include "aquarium.h"
#include "view.h"
#include "../utils.h"

enum movement_pattern { RANDOMWAYPOINT };
enum status { STARTED, NOT_STARTED };

typedef STAILQ_HEAD(tailq, fish_destination) tailq_t;

struct fish_destination {
    struct coordinates destination_coordinates;
    unsigned long long time_at_destination;
    struct view_of_destination *views[MAX_VIEWS + 1]; //Array of views where the fish has to go (NULL terminates the array)
    STAILQ_ENTRY(fish_destination) next;
};

struct view_of_destination {
    char *view_name;
    int is_sent;
};

struct fish {
    char *name;
    struct coordinates top_left;
    tailq_t destinations_queue; // list of destinations
    int speed;
    int height;
    int width;
    enum movement_pattern movement_pattern; // type of movement
    enum status status; // started or not started
    struct fish *next; // next fish in the chained list of fishes
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
int remove_finished_movements(struct aquarium *aquarium, struct fish *fish);
int len_movements_queue(struct fish *fish);
void free_fishes_array(struct fish **fishes, struct view *view);
int add_intermediate_movements(struct aquarium *aquarium, struct fish *fish, struct fish_destination *origin, struct fish_destination *destination, int is_first_call);
struct coordinates *get_intersections_btw_trajectory_and_views(struct view **views, struct coordinates *p1, struct coordinates *p2);
void debug_destinations_queue(FILE *log, struct fish *fish);
// TODO: add test
int coordinates_in_view_not_connected(struct aquarium *aquarium, struct coordinates coordinates);
int mark_destination_as_sent(char *view_name, struct fish_destination *destination);
int destination_sent_to_view(char *view_name, struct fish_destination *destination);
#endif // _FISH_H_ 