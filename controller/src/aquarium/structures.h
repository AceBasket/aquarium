#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#include "../utils.h"
#include <sys/queue.h>
#include <math.h>

// AQUARIUM 
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

// FISH

enum movement_pattern { RANDOMWAYPOINT };
enum status { STARTED, NOT_STARTED };
typedef STAILQ_HEAD(tailq, fish_destination) tailq_t;
struct fish_destination {
    struct coordinates destination_coordinates;
    unsigned long long time_at_destination;
    struct view_of_destination *views[MAX_VIEWS + 1]; // array of views where the fish has to go (NULL terminates the array)
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

// VIEW
struct view {
    char *name;
    int socket_fd;
    struct coordinates top_left;
    int height;
    int width;
    struct view *next; // next view in the chained list of views
    time_t time_last_ping; // last time the view was active
};

#endif // _STRUCTURES_H_ 