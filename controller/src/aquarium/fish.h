#ifndef _FISH_H_
#define _FISH_H_

// #include "view.h"
// #include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "view.h"

/* creates a fish */
struct fish *create_fish(char *name, struct coordinates top_left, int height, int width, enum movement_pattern mvt);
/* adds a fish to the list in aquarium */
int add_fish(struct aquarium *aquarium, struct fish *fish);
/* removes the specified fish from the list in aquarium */
int remove_fish(struct aquarium *aquarium, struct fish *fish);
/* returns the fish associated with the name given in argument */
struct fish *get_fish_from_name(struct aquarium *aquarium, char *name);
/* returns the list of fishes in aquarium */
struct fish *get_fishes(struct aquarium *aquarium);
/* adds a specific destination to a fish */
int add_specific_destination(struct fish *fish, struct fish_destination *destination);
/* returns an array of fishes whose position is inside the specified view */
struct fish **get_fishes_in_view(struct aquarium *aquarium, struct view *view, int started);
/* returns an array of fishes who have at least one destination inside the specified view */
struct fish **get_fishes_with_destination_in_view(struct aquarium *aquarium, struct view *view, int started);
/* returns a boolean on weither the coordinates specified are inside the specified view */
int coordinates_are_in_view(struct coordinates *c, struct view *view);
/* returns the length of the fishes list in aquarium */
int len_fishes(struct aquarium *aquarium);
/* changes fish's status to STARTED */
int start_fish(struct aquarium *aquarium, char *name);
/* returns the distance between two sets of coordinates */
float distance(struct coordinates destination, struct coordinates origin);
/* Adds a random destination to the specified fish */
int add_movement(struct aquarium *aquarium, struct fish *fish);
/* updates the fish's coordinates to its last destination */
int update_fish_coordinates(struct fish *fish);
/* remove destinations that have been sent */
int remove_finished_movements(struct aquarium *aquarium, struct fish *fish);
/* returns the length of the fish's destinations' queue */
int len_movements_queue(struct fish *fish);
/* free an array of fishes returned from get_fishes_with_destination_in_view */
void free_fishes_array(struct fish **fishes, struct view *view);
/* adds the positions on the borders on the views as destinations */
int add_intermediate_movements(struct aquarium *aquarium, struct fish *fish, struct fish_destination *origin, struct fish_destination *destination, int is_first_call);
/* returns an array of coordinates containing the intersections of the trajectory on the views */
struct coordinates *get_intersections_btw_trajectory_and_views(struct view **views, struct coordinates *p1, struct coordinates *p2);
/* prints the content of the destinations_queue */
void debug_destinations_queue(FILE *log, struct fish *fish);
/* returns a true if coordinates are in a view not given to a client */
int coordinates_in_view_not_connected(struct aquarium *aquarium, struct coordinates coordinates);
/* marks a destination as sent */
int mark_destination_as_sent(char *view_name, struct fish_destination *destination);
/* returns true if the specified destination has already been sent to the specified view */
int destination_sent_to_view(char *view_name, struct fish_destination *destination);
/* frees memory allocated to the specified fish */
void free_fish(struct fish *fish);
/* adds the name of the views that handle the area the destination specified is in */
int add_views_to_destination(struct aquarium *aquarium, struct fish_destination *destination);
#endif // _FISH_H_ 