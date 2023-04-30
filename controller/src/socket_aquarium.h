#ifndef _SOCKET_AQUARIUM_H
#define _SOCKET_AQUARIUM_H

struct view *get_view_from_socket(struct aquarium *aquarium, int socket_fd);
int x_coordinate_to_percentage(struct view *view, int x);
int y_coordinate_to_percentage(struct view *view, int y);
int percentage_to_x_coordinate(struct view *view, int percentage);
int percentage_to_y_coordinate(struct view *view, int percentage);
struct coordinates choose_random_coordinates(struct aquarium *aquarium);
struct view *get_first_free_view_socket(struct aquarium *aquarium);

#endif // SOCKET_AQUARIUM_H