#ifndef SOCKET_AQUARIUM_H
#define SOCKET_AQUARIUM_H
struct view *get_view_from_socket(struct aquarium *a, int socket_fd);
int x_coordinate_to_percentage(struct view *v, int x);
int y_coordinate_to_percentage(struct view *v, int y);
struct coordinates choose_random_coordinates(struct aquarium *a);
struct view *get_first_free_view_socket(struct aquarium *a);
#endif // SOCKET_AQUARIUM_H