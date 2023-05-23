#ifndef _VIEW_H_
#define _VIEW_H_

#include <stdlib.h>
#include <time.h>
#include "aquarium.h"
#include "../utils.h"
#include "coordinates.h"


struct view {
    char *name;
    int socket_fd;
    struct coordinates top_left;
    int height;
    int width;
    struct view *next; // next view in the chained list of views
    time_t time_last_ping; // last time the view was active
};
/* creates a view */
struct view *create_view(char *name, struct coordinates top_left, int height, int width);
/* adds a view to the list in aquarium */
int add_view(struct aquarium *aquarium, struct view *view);
/* removes the specified view from the list in aquarium */
int remove_view(struct aquarium *aquarium, struct view *view);
/* returns the view associated with the name specified */
struct view *get_view(struct aquarium *aquarium, char *name);
/* returns an array of views handling the area to which the coordinates belong */
struct view **get_views_from_coordinates(struct aquarium *aquarium, struct coordinates coordinates);
/* returns the number of elements in the list of views in aquarium */
int len_views(struct aquarium *aquarium);
/* frees a view */
void free_view(struct view *view);
/* frees the array of views returned by get_views_from_coordinates() */
int free_views_array(struct view **views);

#endif // _VIEW_H_