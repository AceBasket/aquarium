#include "aquarium.h"
#include <stdlib.h>
struct view *get_view_from_socket(struct aquarium *aquarium, int socket_fd) {
    // if the aquarium is empty, return NULL
    if (aquarium->views == NULL) {
        return NULL;
    }
    // if the aquarium is not empty, check if the view is in the aquarium
    struct view *current = aquarium->views;

    do {
        if (current->socket_fd == socket_fd) {
            // if the view is in the aquarium, return it
            return current;
        }
        current = current->next;
    } while (current != NULL);

    // if the view is not in the aquarium, return NULL
    return NULL;
}

int x_coordinate_to_percentage(struct view *view, int x) {
    return (x - view->top_left.x) * 100 / view->width;
}

int y_coordinate_to_percentage(struct view *view, int y) {
    return (y - view->top_left.y) * 100 / view->height;
}

struct coordinates choose_random_coordinates(struct aquarium *aquarium) {
    struct coordinates c;
    c.x = rand() % aquarium->width;
    c.y = rand() % aquarium->height;
    return c;
}

struct view *get_first_free_view_socket(struct aquarium *aquarium) {
    struct view *view = aquarium->views;
    while (view != NULL) {
        if (view->socket_fd == -1) {
            return view;
        }
        view = view->next;
    }
    return NULL;
}