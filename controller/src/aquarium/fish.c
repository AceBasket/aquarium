#include <stdlib.h>
#include <string.h>
#include "fish.h"
#include "../utils.h"

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
    fish->speed = rand() % 50 + 100;
    fish->next = NULL;
    return fish;
}

void free_fish(struct fish *fish) {
    if (fish == NULL) {
        return;
    }
    struct fish_destination *current_destination = STAILQ_FIRST(&fish->destinations_queue);
    while (current_destination != NULL) {
        STAILQ_REMOVE_HEAD(&fish->destinations_queue, next);
        free(current_destination);
        current_destination = STAILQ_FIRST(&fish->destinations_queue);
    }
    free(fish->name);
    free(fish);
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

int add_specific_destination(struct fish *fish, struct fish_destination *destination) {
    /* Deep copy necessary */
    struct fish_destination *new_destination = malloc(sizeof(struct fish_destination));
    new_destination->destination_coordinates.x = destination->destination_coordinates.x;
    new_destination->destination_coordinates.y = destination->destination_coordinates.y;
    new_destination->time_at_destination = destination->time_at_destination;
    STAILQ_INSERT_TAIL(&fish->destinations_queue, new_destination, next);
    // STAILQ_INSERT_TAIL(&fish->destinations_queue, destination, next);
    return OK;
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
            if (coordinates_are_in_view(&current_fish->top_left, view) == OK) {
                fishes[i] = current_fish;
                i++;
            }

        }
        current_fish = current_fish->next;
    } while (current_fish != NULL);

    fishes[i] = NULL; // end of the array
    return fishes;
}

struct fish **get_fishes_with_destination_in_view(struct aquarium *aquarium, struct view *view, int started) {
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
        int fish_found_bool = NOK;
        if ((started && current_fish->status == STARTED) || !started) {
            // if fish is in view
            // if (coordinates_are_in_view(&current_fish->top_left, view) == OK) {
            //     fishes[i] = current_fish;
            //     i++;
            // } else {
                // if fish is not in view, check if it has a destination in view
            struct fish_destination *current_destination = STAILQ_FIRST(&current_fish->destinations_queue);
            struct fish *fish_with_destination_in_view = create_fish(current_fish->name, current_fish->top_left, current_fish->height, current_fish->width, current_fish->movement_pattern);
            while (current_destination != NULL) {
                if (coordinates_are_in_view(&current_destination->destination_coordinates, view) == OK) {
                    add_specific_destination(fish_with_destination_in_view, current_destination);
                    fish_found_bool = OK;
                    break;
                }
                current_destination = STAILQ_NEXT(current_destination, next);
            }
            if (fish_found_bool == OK) {
                fishes[i] = fish_with_destination_in_view;
                i++;
            } else {
                free_fish(fish_with_destination_in_view);
            }
        // }
        }
        current_fish = current_fish->next;

    } while (current_fish != NULL);
    fishes[i] = NULL; // end of the array
    return fishes;
}

int coordinates_are_in_view(struct coordinates *c, struct view *view) {
    if (c->x >= view->top_left.x && c->x <= view->top_left.x + view->width) {
        if (c->y >= view->top_left.y && c->y <= view->top_left.y + view->height) {
            return OK;
        }
    }
    return NOK;
}

void free_fishes_array(struct fish **fishes, __attribute__((unused))struct view *view) {
    int i = 0;
    while (fishes[i] != NULL) {
        if (coordinates_are_in_view(&fishes[i]->top_left, view) == NOK) {
            free_fish(fishes[i]);
        }
        i++;
    }
    free(fishes);
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

float distance(struct coordinates destination, struct coordinates origin) {
    return sqrt(pow(destination.x - origin.x, 2) + pow(destination.y - origin.y, 2));
}

void debug_destinations_queue(FILE *log, struct fish *fish) {
    struct fish_destination *current_destination = STAILQ_FIRST(&fish->destinations_queue);
    fprintf(log, "destinations queue:\n");
    while (current_destination != NULL) {
        fprintf(log, "%dx%d at %ld\n", current_destination->destination_coordinates.x, current_destination->destination_coordinates.y, current_destination->time_at_destination);
        current_destination = STAILQ_NEXT(current_destination, next);
    }
    fprintf(log, "\n");
    fflush(log);
}

int add_movement(struct aquarium *aquarium, struct fish *fish) {
    struct fish_destination *new_destination = malloc(sizeof(struct fish_destination));
    new_destination->destination_coordinates.x = rand() % aquarium->width; // between 0 and width
    new_destination->destination_coordinates.y = rand() % aquarium->height; // between 0 and height
    time_t time_at_destination_previous_destination = time(NULL);
    if (!STAILQ_EMPTY(&fish->destinations_queue)) {
        struct fish_destination *element = STAILQ_FIRST(&fish->destinations_queue);
        while (STAILQ_NEXT(element, next) != NULL) {
            element = STAILQ_NEXT(element, next);
        }
        time_at_destination_previous_destination = element->time_at_destination;
        add_intermediate_movements(aquarium, fish, element, new_destination);
        // actual time + previous destination time + time to get to this destination
        new_destination->time_at_destination = time_at_destination_previous_destination + (time_t)((distance(new_destination->destination_coordinates, element->destination_coordinates)) / fish->speed);
    } else {
        new_destination->time_at_destination = time_at_destination_previous_destination + (time_t)((distance(new_destination->destination_coordinates, fish->top_left)) / fish->speed);
    }
    STAILQ_INSERT_TAIL(&fish->destinations_queue, new_destination, next);
    return OK;
}

int add_intermediate_movements(struct aquarium *aquarium, struct fish *fish, struct fish_destination *origin, struct fish_destination *destination) {
    struct fish_destination *new_destination;

    /* get views for origin point and destination point */
    struct view **views_origin = get_views_from_coordinates(aquarium, origin->destination_coordinates);
    struct view **views_destination = get_views_from_coordinates(aquarium, destination->destination_coordinates);
    int len_views_origin;
    for (len_views_origin = 0; views_origin[len_views_origin] != NULL; len_views_origin++) {
    }
    int len_views_destination;
    for (len_views_destination = 0; views_destination[len_views_destination] != NULL; len_views_destination++) {
    }
    if (len_views_origin == len_views_destination) {
        int i;
        for (i = 0; i < len_views_origin; i++) {
            if (views_origin[i] != views_destination[i]) {
                // both points don't belong to the same view(s)
                break;
            }
        }
        if (i == len_views_origin) {
            // both points belong to the same view(s)
            return OK;
        }
    }
    struct view *views[len_views_origin + len_views_destination + 1];
    for (int i = 0; i < len_views_origin; i++) {
        views[i] = views_origin[i];
    }
    for (int i = 0; i < len_views_destination; i++) {
        views[i + len_views_origin] = views_destination[i];
    }
    views[len_views_origin + len_views_destination] = NULL;

    /* get intersections for each view found above */
    struct coordinates *intersections = get_instersections_btw_trajectory_and_views(views, &origin->destination_coordinates, &destination->destination_coordinates);
    struct fish_destination *origin_correct_pointer;
    STAILQ_FOREACH(origin_correct_pointer, &fish->destinations_queue, next) {
        if (origin_correct_pointer->destination_coordinates.x == origin->destination_coordinates.x && origin_correct_pointer->destination_coordinates.y == origin->destination_coordinates.y) {
            // we need to get the correct pointer to insert after
            origin = origin_correct_pointer;
        }
    }
    for (int i = 0; intersections[i].x != -1 && i < len_views_destination + len_views_origin + 1; i++) {
        if (intersections[i].x == origin->destination_coordinates.x && intersections[i].y == origin->destination_coordinates.y) {
            // if the intersection is the same as the one before, we don't add it
            continue;
        } else if (intersections[i].x < 0 || intersections[i].y < 0) {
            // if the intersection is not in the aquarium, we don't add it
            continue;
        }
        new_destination = malloc(sizeof(struct fish_destination));
        new_destination->destination_coordinates.x = intersections[i].x;
        new_destination->destination_coordinates.y = intersections[i].y;
        new_destination->time_at_destination = origin->time_at_destination + (time_t)((distance(new_destination->destination_coordinates, origin->destination_coordinates)) / fish->speed);
        STAILQ_INSERT_AFTER(&fish->destinations_queue, origin, new_destination, next);
        origin = new_destination; // update origin
    }
    free(intersections);
    return OK;
}

struct coordinates *get_instersections_btw_trajectory_and_views(struct view **views, struct coordinates *p1, struct coordinates *p2) {
    int len_views;
    for (len_views = 0; views[len_views] != NULL; len_views++) {
    }
    struct coordinates *intersections = malloc(sizeof(struct coordinates) * len_views * 2); // to make sure to have enough space

    if (p2->x == p1->x) {
        for (int i = 0; i < len_views; i++) {
            intersections[i].x = p1->x;
            intersections[i].y = views[i]->top_left.y;
        }
        return intersections;
    }
    float slope_trajectory = (float)(p2->y - p1->y) / (float)(p2->x - p1->x); // a in y = ax+ b
    float y_intercept_trajectory = p1->y - slope_trajectory * p1->x; // b in y = ax+ b
    int intersections_index = 0;
    int views_index;
    for (views_index = 0; views_index < len_views; views_index++) {
        int intersection_filled = 0;
        float top_side_intersection = (views[views_index]->top_left.y - y_intercept_trajectory) / slope_trajectory;
        float bottom_side_intersection = (views[views_index]->top_left.y + views[views_index]->height - y_intercept_trajectory) / slope_trajectory;
        float left_side_intersection = slope_trajectory * views[views_index]->top_left.x + y_intercept_trajectory;
        float right_side_intersection = slope_trajectory * (views[views_index]->top_left.x + views[views_index]->width) + y_intercept_trajectory;
        /* Coordinates are cast to int ==> rounds down */
        float distance_origin_destination = distance(*p1, *p2);
        if (top_side_intersection <= views[views_index]->top_left.x + views[views_index]->width) {
            if (distance((struct coordinates) { top_side_intersection, views[views_index]->top_left.y }, *p1) < distance_origin_destination &&distance((struct coordinates) { top_side_intersection, views[views_index]->top_left.y }, *p2) < distance_origin_destination) {
                intersections[intersections_index].x = top_side_intersection;
                intersections[intersections_index].y = views[views_index]->top_left.y;
                intersection_filled = 1;
                intersections_index++;
            }
        }
        if (bottom_side_intersection <= views[views_index]->top_left.x + views[views_index]->width) {
            if (distance((struct coordinates) { bottom_side_intersection, views[views_index]->top_left.y + views[views_index]->height }, *p1) < distance_origin_destination &&distance((struct coordinates) { bottom_side_intersection, views[views_index]->top_left.y + views[views_index]->height }, *p2) < distance_origin_destination) {
                intersections[intersections_index].x = bottom_side_intersection;
                intersections[intersections_index].y = views[views_index]->top_left.y + views[views_index]->height;
                intersection_filled = 1;
                intersections_index++;
            }
        }
        if (left_side_intersection <= views[views_index]->top_left.y + views[views_index]->height) {
            if (distance((struct coordinates) { views[views_index]->top_left.x, left_side_intersection }, *p1) < distance_origin_destination &&distance((struct coordinates) { views[views_index]->top_left.x, left_side_intersection }, *p2) < distance_origin_destination) {
                intersections[intersections_index].x = views[views_index]->top_left.x;
                intersections[intersections_index].y = left_side_intersection;
                intersection_filled = 1;
                intersections_index++;
            }
        }
        if (right_side_intersection <= views[views_index]->top_left.y + views[views_index]->height) {
            if (distance((struct coordinates) { views[views_index]->top_left.x + views[views_index]->width, right_side_intersection }, *p1) < distance_origin_destination &&distance((struct coordinates) { views[views_index]->top_left.x + views[views_index]->width, right_side_intersection }, *p2) < distance_origin_destination) {
                intersections[intersections_index].x = views[views_index]->top_left.x + views[views_index]->width;
                intersections[intersections_index].y = right_side_intersection;
                intersection_filled = 1;
                intersections_index++;
            }
        }
        if (!intersection_filled) {
            intersections[intersection_filled].x = -1;
            intersections[intersection_filled].y = -1;
            intersection_filled++;
        }
    }
    for (int i = views_index; i < len_views * 2; i++) { // fill the rest with -1
        intersections[i].x = -1;
        intersections[i].y = -1;
    }

    /* Now he have to sort the intersections */
    int tmp;
    for (int i = 1; i < len_views; i++) {
        for (int j = i; j > 0; j--) {
            if (distance(*p1, intersections[j]) < distance(*p1, intersections[j - 1])) {
                tmp = intersections[j].x;
                intersections[j].x = intersections[j - 1].x;
                intersections[j - 1].x = tmp;
                tmp = intersections[j].y;
                intersections[j].y = intersections[j - 1].y;
                intersections[j - 1].y = tmp;
            }
        }
    }

    return intersections;
}


int update_fish_coordinates(struct fish *fish) {
    struct fish_destination *current_destination = STAILQ_FIRST(&fish->destinations_queue);
    if (current_destination == NULL) {
        return NOK;
    }
    fish->top_left.x = current_destination->destination_coordinates.x;
    fish->top_left.y = current_destination->destination_coordinates.y;
    return OK;
}

int remove_finished_movements(struct fish *fish) {
    struct fish_destination *current_destination = STAILQ_FIRST(&fish->destinations_queue);

    while (current_destination != NULL) {
        if (current_destination->time_at_destination <= time(NULL)) {
            if (update_fish_coordinates(fish) == NOK) {
                return NOK;
            };
            STAILQ_REMOVE_HEAD(&fish->destinations_queue, next);
            free(current_destination);
            current_destination = STAILQ_FIRST(&fish->destinations_queue);
        } else {
            break;
        }
    }
    return OK;
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