#ifndef _SOCKET_AQUARIUM_H
#define _SOCKET_AQUARIUM_H

#include <stdlib.h>
#include "../aquarium/view.h"

/* returns the view associated with the socket */
struct view *get_view_from_socket(struct aquarium *aquarium, int socket_fd);
/* converts the x coordinate into a percentage */
int x_coordinate_to_percentage(struct view *view, int x);
/* converts the y coordinate into a percentage */
int y_coordinate_to_percentage(struct view *view, int y);
/* converts the pourcentage into the x coordinate */
int percentage_to_x_coordinate(struct view *view, int percentage);
/* converts the pourcentage into the y coordinate */
int percentage_to_y_coordinate(struct view *view, int percentage);
/* selects random coordinates */
struct coordinates choose_random_coordinates(struct aquarium *aquarium);
/* returns the fisrt view whose socket is free */
struct view *get_first_free_view_socket(struct aquarium *aquarium);

#endif // _SOCKET_AQUARIUM_H