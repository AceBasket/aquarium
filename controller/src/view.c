#include "view.h"
// #include "utils.h"

struct view *create_view(char *name, struct coordinates top_left, int width, int height) {
    // create a new view
    struct view *view = malloc(sizeof(struct view));
    exit_if(view == NULL, "malloc failed");
    view->top_left = top_left;
    view->height = height;
    view->width = width;
    view->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(view->name, name);
    view->next = NULL;
    view->socket_fd = -1; //to store the socket file descriptor
    view->time_last_ping = time(NULL);
    return view;
}

int add_view(struct aquarium *aquarium, struct view *view) {
    // if the aquarium is empty, add the view
    if (aquarium->views == NULL) {
        aquarium->views = view;
        return OK;
    }
    // if the aquarium is not empty, check if the view is already in the aquarium

    struct view *current = aquarium->views;

    if (strcmp(current->name, view->name) == 0) {
        return NOK;
    }

    while (current->next != NULL) {
        current = current->next;
        if (strcmp(current->name, view->name) == 0) {
            return NOK;
        }
    }
    // if the view is not in the aquarium, add it to the end of the list
    current->next = view;
    return OK;
}

int remove_view(struct aquarium *aquarium, struct view *view) {
    // if the aquarium is empty, return failure
    if (aquarium->views == NULL) {
        return NOK;
    }

    struct view *current = aquarium->views;
    if (strcmp(current->name, view->name) == 0) {
        // if the view is in the aquarium, remove it
        aquarium->views = current->next;
        free(current->name);
        free(current);
        return OK;
    }
    struct view *previous = NULL;
    while (current->next != NULL) {
        if (strcmp(current->name, view->name) == 0) {
            // if the view is in the aquarium, remove it
            if (previous == NULL) {
                aquarium->views = current->next;
            } else {
                previous->next = current->next;
            }
            free(current->name);
            free(current);
            return OK;
        }
        previous = current;
        current = current->next;
    }
    // if the view is not in the aquarium, return failure
    return NOK;
}

struct view *get_view(struct aquarium *aquarium, char *name) {
    // if the aquarium is empty, return NULL
    if (aquarium->views == NULL) {
        return NULL;
    }
    // if the aquarium is not empty, check if the view is in the aquarium
    struct view *current = aquarium->views;

    do {
        if (strcmp(current->name, name) == 0) {
            // if the view is in the aquarium, return it
            return current;
        }
        current = current->next;
    } while (current != NULL);

    // if the view is not in the aquarium, return NULL
    return NULL;
}

int len_views(struct aquarium *aquarium) {

    // if the aquarium is not empty
    if (aquarium->views == NULL) {
        return 0;
    }

    int nb_views = 1;

    struct view *current = aquarium->views;

    while (current->next != NULL) {
        current = current->next;
        nb_views++;
    }
    return nb_views;
}