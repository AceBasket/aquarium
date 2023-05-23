#include <stdlib.h>
#include <string.h>
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
    fish->speed = rand() % 50 + 50;
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
        for (int i = 0; current_destination->views[i] != NULL; i++) {
            free(current_destination->views[i]->view_name);
            free(current_destination->views[i]);
        }
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

    // deep copy of views that the destination belongs to
    for (int i = 0; destination->views[i] != NULL; i++) {
        new_destination->views[i] = malloc(sizeof(struct view_of_destination));
        new_destination->views[i]->view_name = malloc(sizeof(char) * (strlen(destination->views[i]->view_name) + 1));
        strcpy(new_destination->views[i]->view_name, destination->views[i]->view_name);
        new_destination->views[i]->is_sent = destination->views[i]->is_sent;
    }
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
            for (struct fish_destination *current_destination = STAILQ_FIRST(&current_fish->destinations_queue); current_destination != NULL; current_destination = STAILQ_NEXT(current_destination, next)) {
                for (int j = 0; current_destination->views[j] != NULL; j++) {
                    // if the destination belongs to the view and has not been sent yet
                    if ((strcmp(current_destination->views[j]->view_name, view->name) == 0) && (current_destination->views[j]->is_sent == NOK)) {
                        fishes[i] = current_fish;
                        fish_found_bool = OK;
                        break;
                    }
                }
                if (fish_found_bool == OK) {
                    break;
                }
            }
            if (fish_found_bool == OK) {
                i++;
            }
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
    // TODO: think about what needs to be freed
    // int i = 0;
    // while (fishes[i] != NULL) {
    //     if (coordinates_are_in_view(&fishes[i]->top_left, view) == NOK) {
    //         free_fish(fishes[i]);
    //     }
    //     i++;
    // }
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
        if (current_destination->views[0] == NULL) {
            fprintf(log, "%dx%d at %lld (no view)\n", current_destination->destination_coordinates.x, current_destination->destination_coordinates.y, current_destination->time_at_destination);
            current_destination = STAILQ_NEXT(current_destination, next);
            continue;
        }
        fprintf(log, "%dx%d at %lld in view %s (at least)\n", current_destination->destination_coordinates.x, current_destination->destination_coordinates.y, current_destination->time_at_destination, current_destination->views[0]->view_name);
        current_destination = STAILQ_NEXT(current_destination, next);
    }
    fprintf(log, "\n");
    fflush(log);
}

int add_views_to_destination(struct aquarium *aquarium, struct fish_destination *destination) {
    struct view **views = get_views_from_coordinates(aquarium, destination->destination_coordinates);
    int len_views;
    for (len_views = 0; views[len_views] != NULL; len_views++) {
    }
    for (int i = 0; i < len_views; i++) {
        destination->views[i] = malloc(sizeof(struct view_of_destination));
        destination->views[i]->view_name = malloc(sizeof(char) * (strlen(views[i]->name) + 1));
        strcpy(destination->views[i]->view_name, views[i]->name);
        destination->views[i]->is_sent = NOK;
    }
    destination->views[len_views] = NULL;
    free(views);
    return OK;
}

int add_movement(struct aquarium *aquarium, struct fish *fish) {
    struct fish_destination *new_destination = malloc(sizeof(struct fish_destination));
    new_destination->destination_coordinates.x = rand() % aquarium->width; // between 0 and width
    new_destination->destination_coordinates.y = rand() % aquarium->height; // between 0 and height
    // unsigned long long time_at_destination_previous_destination = get_time_in_milliseconds();
    struct fish_destination *element;
    if (!STAILQ_EMPTY(&fish->destinations_queue)) {
        element = STAILQ_FIRST(&fish->destinations_queue);
        // Get the last element of the queue
        while (STAILQ_NEXT(element, next) != NULL) {
            element = STAILQ_NEXT(element, next);
        }
        // add all the positions on the borders of the views on the path from origin to destination
        add_intermediate_movements(aquarium, fish, element, new_destination, NOK);
    } else {
        // if the queue is empty, the fish is at the origin and we can create a mock fish destination
        element = malloc(sizeof(struct fish_destination));
        element->destination_coordinates = fish->top_left;
        element->time_at_destination = get_time_in_milliseconds();
        // add all the positions on the borders of the views on the path from origin to destination
        add_intermediate_movements(aquarium, fish, element, new_destination, OK);
    }

    /* Now that new destinations have been added to tail of element, we need to update element */
    if (!STAILQ_EMPTY(&fish->destinations_queue)) {
        // Go through the queue again to get the new element last element
        element = STAILQ_FIRST(&fish->destinations_queue);
        while (STAILQ_NEXT(element, next) != NULL) {
            element = STAILQ_NEXT(element, next);
        }
    }
    // get time_at_destination_previous_destination
    unsigned long long time_at_destination_previous_destination = element->time_at_destination;
    // compute time needed to get to destination from previous destination
    float time_to_get_to_new_destination = (distance(new_destination->destination_coordinates, element->destination_coordinates)) / fish->speed;
    /* Forbid to have less than a second between two destinations */
    new_destination->time_at_destination = (time_to_get_to_new_destination < 1) ? add_seconds_to_time_in_milliseconds(time_at_destination_previous_destination, 1) : add_seconds_to_time_in_milliseconds(time_at_destination_previous_destination, round(time_to_get_to_new_destination));

// adding all the views to which the destination belongs to
    add_views_to_destination(aquarium, new_destination);

    STAILQ_INSERT_TAIL(&fish->destinations_queue, new_destination, next);
    return OK;
}

int add_intermediate_movements(struct aquarium *aquarium, struct fish *fish, struct fish_destination *origin, struct fish_destination *destination, int is_first_call) {
    struct fish_destination *new_destination;

    // TODO: might not have to do this now that info is in fish_destination
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
    struct coordinates *intersections = get_intersections_btw_trajectory_and_views(views, &origin->destination_coordinates, &destination->destination_coordinates);

    /* So that we can test the function */
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

        // compute time needed to get to destination from previous destination
        float time_to_get_to_new_destination = (distance(new_destination->destination_coordinates, origin->destination_coordinates)) / fish->speed;
// printf("(add intermediate movements) %f to get from %dx%d to %dx%d\n", time_to_get_to_new_destination, origin->destination_coordinates.x, origin->destination_coordinates.y, new_destination->destination_coordinates.x, new_destination->destination_coordinates.y);
        // forbid to have less than a second between two destinations
        new_destination->time_at_destination = (time_to_get_to_new_destination < 1) ? add_seconds_to_time_in_milliseconds(origin->time_at_destination, 1) : add_seconds_to_time_in_milliseconds(origin->time_at_destination, (int)round(time_to_get_to_new_destination));
// printf("=> %ld\n", new_destination->time_at_destination);
        // Adding all the views to which the destination belongs to
        add_views_to_destination(aquarium, new_destination);

        if (is_first_call == OK && i < 1) { // if no destination in queue, insert at head
            STAILQ_INSERT_HEAD(&fish->destinations_queue, new_destination, next);
        } else {
            STAILQ_INSERT_AFTER(&fish->destinations_queue, origin, new_destination, next);
        }

        origin = new_destination; // update origin
    }
    free(intersections);
    return OK;
}

struct coordinates *get_intersections_btw_trajectory_and_views(struct view **views, struct coordinates *p1, struct coordinates *p2) {
    int len_views;
    for (len_views = 0; views[len_views] != NULL; len_views++) {
        printf("view %s\n", views[len_views]->name);
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

        /* Coordinates are rounded to avoid float imprecision */
        float distance_origin_destination = distance(*p1, *p2);
        if (top_side_intersection <= views[views_index]->top_left.x + views[views_index]->width) {
            if (distance((struct coordinates) { round(top_side_intersection), views[views_index]->top_left.y }, *p1) < distance_origin_destination &&distance((struct coordinates) { round(top_side_intersection), views[views_index]->top_left.y }, *p2) < distance_origin_destination) {
                intersections[intersections_index].x = round(top_side_intersection);
                intersections[intersections_index].y = views[views_index]->top_left.y;
                intersection_filled = 1;
                intersections_index++;
            }
        }
        if (bottom_side_intersection <= views[views_index]->top_left.x + views[views_index]->width) {
            if (distance((struct coordinates) { round(bottom_side_intersection), views[views_index]->top_left.y + views[views_index]->height }, *p1) < distance_origin_destination &&distance((struct coordinates) { round(bottom_side_intersection), views[views_index]->top_left.y + views[views_index]->height }, *p2) < distance_origin_destination) {
                intersections[intersections_index].x = round(bottom_side_intersection);
                intersections[intersections_index].y = views[views_index]->top_left.y + views[views_index]->height;
                intersection_filled = 1;
                intersections_index++;
            }
        }
        if (left_side_intersection <= views[views_index]->top_left.y + views[views_index]->height) {
            if (distance((struct coordinates) { views[views_index]->top_left.x, round(left_side_intersection) }, *p1) < distance_origin_destination &&distance((struct coordinates) { views[views_index]->top_left.x, round(left_side_intersection) }, *p2) < distance_origin_destination) {
                intersections[intersections_index].x = views[views_index]->top_left.x;
                intersections[intersections_index].y = round(left_side_intersection);
                intersection_filled = 1;
                intersections_index++;
            }
        }
        if (right_side_intersection <= views[views_index]->top_left.y + views[views_index]->height) {
            if (distance((struct coordinates) { views[views_index]->top_left.x + views[views_index]->width, round(right_side_intersection) }, *p1) < distance_origin_destination &&distance((struct coordinates) { views[views_index]->top_left.x + views[views_index]->width, round(right_side_intersection) }, *p2) < distance_origin_destination) {
                intersections[intersections_index].x = views[views_index]->top_left.x + views[views_index]->width;
                intersections[intersections_index].y = round(right_side_intersection);
                intersection_filled = 1;
                intersections_index++;
            }
        }
        if (!intersection_filled) {
            intersections[intersections_index].x = -1;
            intersections[intersections_index].y = -1;
            intersections_index++;
        }
    }
    for (int i = intersections_index; i < len_views * 2; i++) { // fill the rest with -1
        intersections[i].x = -1;
        intersections[i].y = -1;
    }

    /* Placing all the (-1x-1) at the end of the array */
    int index = 0;
    for (int i = 0; i < len_views * 2; i++) {
        if (intersections[i].x != -1 && intersections[i].y != -1) {
            struct coordinates temp = intersections[index];
            intersections[index] = intersections[i];
            intersections[i] = temp;
            index++;
        }
    }

    printf("intersections:\n");
    for (int i = 0; i < len_views * 2; i++) {
        printf("%dx%d\n", intersections[i].x, intersections[i].y);
    }

    /* Sorting the list of intersections by growing distance to origin (but leave the (-1x-1) at the end of the array)*/
    int len_intersections = 0;
    for (len_intersections = 0; len_intersections < len_views * 2 && intersections[len_intersections].x != -1 && intersections[len_intersections].y != -1; len_intersections++) {
    }
    for (int i = 0; i < len_intersections - 1; i++) {
        for (int j = 0; j < len_intersections - i - 1; j++) {
            if (distance(intersections[j], *p1) > distance(intersections[j + 1], *p1)) {
                struct coordinates temp = intersections[j];
                intersections[j] = intersections[j + 1];
                intersections[j + 1] = temp;
            }
        }
    }

    printf("intersections:\n");
    for (int i = 0; i < len_intersections; i++) {
        printf("%dx%d\n", intersections[i].x, intersections[i].y);
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

int coordinates_in_view_not_connected(struct aquarium *aquarium, struct coordinates coordinates) {
    struct view **views = get_views_from_coordinates(aquarium, coordinates);
    int len_views;
    for (len_views = 0; views[len_views] != NULL; len_views++) {
    }
    if (len_views >= 1) {
        return OK;
    }
    return NOK;
}

int destination_sent_to_all_views(struct aquarium *aquarium, struct fish_destination *destination) {
    for (int i = 0; destination->views[i] != NULL; i++) {
        if (destination->views[i]->is_sent == NOK && coordinates_in_view_not_connected(aquarium, destination->destination_coordinates) == NOK) {
            return NOK;
        }
    }
    return OK;
}

int remove_finished_movements(struct aquarium *aquarium, struct fish *fish) {
    struct fish_destination *current_destination = STAILQ_FIRST(&fish->destinations_queue);

    while (current_destination != NULL) {
        // TODO: which one to choose ?
        // if (destination_sent_to_all_views(aquarium, current_destination) == OK) {
        if (current_destination->time_at_destination <= get_time_in_milliseconds() && (destination_sent_to_all_views(aquarium, current_destination) == OK)) {
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

int mark_destination_as_sent(char *view_name, struct fish_destination *destination) {
    for (int i = 0; destination->views[i] != NULL; i++) {
        if (strcmp(destination->views[i]->view_name, view_name) == 0) {
            destination->views[i]->is_sent = OK;
            return OK;
        }
    }
    return NOK;
}

int destination_sent_to_view(char *view_name, struct fish_destination *destination) {
    if (destination == NULL) {
        return NOK;
    }
    for (int i = 0; destination->views[i] != NULL; i++) {
        if (strcmp(destination->views[i]->view_name, view_name) == 0) {
            return destination->views[i]->is_sent;
        }
    }
    // if the view is not in the destination --> no need to send it
    return OK;
}