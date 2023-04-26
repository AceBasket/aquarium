#include "aquarium.h"
#include "utils.h"

int add_fish(struct aquarium *aquarium, struct fish *fish) {
    // if the aquarium is empty, add the fish
    if (aquarium->fishes == NULL) {
        aquarium->fishes = fish;
        return OK;
    }

    // if the aquarium is not empty, check if the fish is already in the aquarium
    struct fish *current = aquarium->fishes;

    //  if only one element in the list
    if (strcmp(current->name, fish->name) == 0) {
        return NOK;
    }



    while (current->next != NULL) {
        current = current->next;
        if (strcmp(current->name, fish->name) == 0) {
            return NOK;
        }
    }
    // if the fish is not in the aquarium, add it to the end of the list
    current->next = fish;
    return OK;
}

int remove_fish(struct aquarium *aquarium, struct fish *fish) {
    // if the aquarium is empty, return failure
    if (aquarium->fishes == NULL) {
        return NOK;
    }
    // if the aquarium is not empty, check if the fish is in the aquarium
    struct fish *current = aquarium->fishes;

    if (strcmp(current->name, fish->name) == 0) {
        aquarium->fishes = current->next;
        free(current->name);
        free(current);
        return OK;
    }

    struct fish *previous = NULL;
    while (current->next != NULL) {
        if (strcmp(current->name, fish->name) == 0) {
            // if the fish is in the aquarium, remove it
            if (previous == NULL) {
                aquarium->fishes = current->next;
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
    // if the fish is not in the aquarium, return failure
    return NOK;
}

struct fish *create_fish(char *name, struct coordinates top_left, int height, int width, enum movement_pattern mvt) {
    // create a new fish
    struct fish *fish = malloc(sizeof(struct fish));
    exit_if(fish == NULL, "malloc failed");
    fish->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(fish->name, name);
    fish->top_left = top_left;
    fish->height = height;
    fish->width = width;
    fish->movement_pattern = mvt;
    fish->status = NOT_STARTED;
    fish->destination = top_left;
    fish->time_to_destination = 0;
    fish->next = NULL;
    return fish;
}

struct fish *get_fish_from_name(struct aquarium *aquarium, char *name) {
    // if the aquarium is empty, return NULL
    if (aquarium->fishes == NULL) {
        return NULL;
    }
    // if the aquarium is not empty, check if the fish is in the aquarium
    struct fish *current = aquarium->fishes;

    do {
        if (strcmp(current->name, name) == 0) {
            // if the fish is in the aquarium, return it
            return current;
        }
        current = current->next;
    } while (current != NULL);

    // if the fish is not in the aquarium, return NULL
    return NULL;
}

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

struct aquarium *create_aquarium(int width, int height) {
    // create a new aquarium
    struct aquarium *aquarium = malloc(sizeof(struct aquarium));
    exit_if(aquarium == NULL, "malloc failed");
    aquarium->fishes = NULL;
    aquarium->views = NULL;
    aquarium->height = height;
    aquarium->width = width;
    return aquarium;
}

struct fish *get_fishes(struct aquarium *aquarium) {
    return aquarium->fishes;
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

void show_aquarium(struct aquarium *aquarium, FILE *output) {
    fprintf(output, "%dx%d\n", aquarium->width, aquarium->height);
    fflush(output);

    struct view *current_view;
    current_view = aquarium->views;
    do {
        fprintf(output, "%s %dx%d+%d+%d\n", current_view->name, current_view->top_left.x, current_view->top_left.y, current_view->width, current_view->height);
        fflush(output);
        current_view = current_view->next;

    } while (current_view != NULL);

}

int len_fishes(struct aquarium *aquarium) {
    // if the aquarium is empty
    if (aquarium->fishes == NULL) {
        return 0;
    }
    int nb_fishes = 1;
    // if the aquarium is not empty
    struct fish *current = aquarium->fishes;

    while (current->next != NULL) {
        current = current->next;
        nb_fishes++;
    }

    return nb_fishes;
}

void save_aquarium(struct aquarium *aquarium, char *name) {
    FILE *f = fopen(name, "w");
    show_aquarium(aquarium, f);
    fclose(f);
}

int start_fish(struct aquarium *aquarium, char *name) {
    struct fish *fish = get_fish_from_name(aquarium, name);
    if (fish != NULL) {
        fish->status = STARTED;
        return OK;
    }
    return NOK;
}

struct fish **get_fishes_in_view(struct aquarium *aquarium, struct view *view, int started) {
    struct fish **fishes = malloc(sizeof(struct fish *) * (len_fishes(aquarium) + 1));
    exit_if(fishes == NULL, "malloc failed");
    exit_if(view == NULL, "view is NULL");
    int i = 0;
    struct fish *current_fish = aquarium->fishes;

    if (current_fish == NULL) {
        fishes[i] = NULL;
        return fishes;
    }

    do {
        if ((started && current_fish->status == STARTED) || !started) {
            if (current_fish->top_left.x >= view->top_left.x && current_fish->top_left.x <= view->top_left.x + view->width) {
                if (current_fish->top_left.y >= view->top_left.y && current_fish->top_left.y <= view->top_left.y + view->height) {
                    fishes[i] = current_fish;
                    i++;
                }
            }
        }
        current_fish = current_fish->next;
    } while (current_fish != NULL);

    fishes[i] = NULL; // end of the array
    return fishes;
}

void set_movement(struct aquarium *aquarium, struct fish *fish) {
    fish->destination.x = rand() % aquarium->width; // between 0 and width
    fish->destination.y = rand() % aquarium->height; // between 0 and height
    fish->time_to_destination = time(NULL) + rand() % 10 + 1; // between 1 and 10
    exit_if(fish->time_to_destination == -1, "time failed");
}

int free_aquarium(struct aquarium *aquarium) {
    if (aquarium == NULL) {
        return OK;
    }
    int fishes_len = len_fishes(aquarium);
    if (fishes_len != 0) {
        // free the fishes
        struct fish *current_fish = aquarium->fishes;
        struct fish *next_fish = current_fish->next;
        for (int i = 0; i < fishes_len; i++) {
            free(current_fish->name);
            free(current_fish);
            current_fish = next_fish;
            if (current_fish != NULL) {
                next_fish = current_fish->next;
            }
        }
    }

    if (aquarium->views != NULL) {
        // free the views
        struct view *current_view = aquarium->views;
        struct view *next_view = current_view->next;
        int views_len = len_views(aquarium);
        for (int i = 0; i < views_len; i++) {
            free(current_view->name);
            free(current_view);
            current_view = next_view;

            if (current_view != NULL) {
                next_view = current_view->next;
            }
        }
    }

    // free the aquarium
    free(aquarium);
    return OK;
}