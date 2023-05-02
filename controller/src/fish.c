#include "fish.h"

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
    tailq_t destination_tmp = STAILQ_HEAD_INITIALIZER(fish->destinations_queue);
    fish->destinations_queue = destination_tmp;
    fish->speed = rand() % 10 + 1;
    fish->next = NULL;
    return fish;
}

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

    if (strcmp(aquarium->fishes->name, fish->name) == 0) {
        aquarium->fishes = aquarium->fishes->next;
        return OK;
    }
    // if the aquarium is not empty, check if the fish is in the aquarium
    struct fish *current = aquarium->fishes;
    struct fish *previous = current;
    do {
        if (strcmp(current->name, fish->name) == 0) {
            // if the fish is in the aquarium, remove it
            previous->next = current->next;
            free(current->name);
            free(current);
            return OK;
        }
        previous = current;
        current = current->next;
    } while (current != NULL);

    // if the fish is not in the aquarium, return failure
    return NOK;
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

struct fish *get_fishes(struct aquarium *aquarium) {
    return aquarium->fishes;
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

int start_fish(struct aquarium *aquarium, char *name) {
    struct fish *fish = get_fish_from_name(aquarium, name);
    if (fish != NULL) {
        fish->status = STARTED;
        return OK;
    }
    return NOK;
}

int distance(struct coordinates destination) {
    return sqrt(pow(destination.x, 2) + pow(destination.y, 2));
}

void add_movement(struct aquarium *aquarium, struct fish *fish) {
    struct fish_destination *new_destination = malloc(sizeof(struct fish_destination));
    new_destination->destination_coordinates.x = rand() % aquarium->width; // between 0 and width
    new_destination->destination_coordinates.y = rand() % aquarium->height; // between 0 and height
    new_destination->time_at_destination = time(NULL) + (distance(new_destination->destination_coordinates)) / fish->speed;
    STAILQ_INSERT_TAIL(&fish->destinations_queue, new_destination, next);
    // exit_if(fish->time_at_destination == -1, "time failed");
}

void remove_finished_movements(struct fish *fish) {
    struct fish_destination *current_destination = STAILQ_FIRST(&fish->destinations_queue);
    while (current_destination != NULL) {
        if (current_destination->time_at_destination <= time(NULL)) {
            STAILQ_REMOVE_HEAD(&fish->destinations_queue, next);
            free(current_destination);
            current_destination = STAILQ_FIRST(&fish->destinations_queue);
        } else {
            break;
        }
    }
}

int len_movements_queue(struct fish *fish) {
    int len = 0;
    struct fish_destination *current_destination = STAILQ_FIRST(&fish->destinations_queue);
    while (current_destination != NULL) {
        len++;
        current_destination = STAILQ_NEXT(current_destination, next);
    }
    return len;
}