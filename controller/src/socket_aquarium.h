#ifndef SOCKET_AQUARIUM_H
#define SOCKET_AQUARIUM_H
struct view *get_view_from_socket(struct aquarium *aquarium, int socket_fd);
int x_coordinate_to_percentage(struct view *view, int x);
int y_coordinate_to_percentage(struct view *view, int y);
struct coordinates choose_random_coordinates(struct aquarium *aquarium);
struct view *get_first_free_view_socket(struct aquarium *aquarium);
#endif // SOCKET_AQUARIUM_H