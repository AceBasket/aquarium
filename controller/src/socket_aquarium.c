#include "aquarium.h"
#include <stdlib.h>
struct view *get_view_from_socket(struct aquarium *a, int socket_fd) {
    // if the aquarium is empty, return NULL
    if (a->views == NULL) {
        return NULL;
    }
    // if the aquarium is not empty, check if the view is in the aquarium
    struct view *current = a->views;
    if (current->socket_fd == socket_fd) {
        // if the view is in the aquarium, return it
        return current;
    }
    while (current->next != NULL) {
        if (current->socket_fd == socket_fd) {
            // if the view is in the aquarium, return it
            return current;
        }
        current = current->next;
    }
    // if the view is not in the aquarium, return NULL
    return NULL;
}

int x_coordinate_to_percentage(struct view *v, int x) {
    return (x - v->top_left.x) * 100 / v->width;
}

int y_coordinate_to_percentage(struct view *v, int y) {
    return (y - v->top_left.y) * 100 / v->height;
}

struct coordinates choose_random_coordinates(struct aquarium *a) {
    struct coordinates c;
    c.x = rand() % a->width;
    c.y = rand() % a->height;
    return c;
}

struct view *get_first_free_view_socket(struct aquarium *a) {
    struct view *v = a->views;
    while (v != NULL) {
        if (v->socket_fd == -1) {
            return v;
        }
        v = v->next;
    }
    return NULL;
}