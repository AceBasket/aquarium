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

struct fish **get_fishes_in_view_and_with_destination_in_view(struct aquarium *aquarium, struct view *view, int started) {
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
            if ((current_fish->top_left.x >= view->top_left.x && current_fish->top_left.x <= view->top_left.x + view->width) && (current_fish->top_left.y >= view->top_left.y && current_fish->top_left.y <= view->top_left.y + view->height)) {
                fishes[i] = current_fish;
                i++;
            } else {
                struct fish_destination *current_destination = STAILQ_FIRST(&current_fish->destinations_queue);
                struct fish *fish_with_destination_in_view = create_fish(current_fish->name, current_fish->top_left, current_fish->height, current_fish->width, current_fish->movement_pattern);
                while (current_destination != NULL) {
                    if ((current_destination->destination_coordinates.x >= view->top_left.x && current_destination->destination_coordinates.x <= view->top_left.x + view->width) && (current_destination->destination_coordinates.y >= view->top_left.y && current_destination->destination_coordinates.y <= view->top_left.y + view->height)) {
                        add_specific_destination(fish_with_destination_in_view, current_destination);
                        break;
                    }
                    current_destination = STAILQ_NEXT(current_destination, next);
                }
                fishes[i] = fish_with_destination_in_view;
                i++;
            }
        }
        current_fish = current_fish->next;

    } while (current_fish != NULL);
    fishes[i] = NULL; // end of the array
    return fishes;
}

int fish_is_in_view(struct fish *fish, struct view *view) {
    if (fish->top_left.x >= view->top_left.x && fish->top_left.x <= view->top_left.x + view->width) {
        if (fish->top_left.y >= view->top_left.y && fish->top_left.y <= view->top_left.y + view->height) {
            return OK;
        }
    }
    return NOK;
}

void free_fishes_array(struct fish **fishes, struct view *view) {
    int i = 0;
    while (fishes[i] != NULL) {
        if (fish_is_in_view(fishes[i], view) == NOK) {
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
    }
    /* actual time + previous destination time + time to get to this destination */
    new_destination->time_at_destination = time_at_destination_previous_destination + (time_t)((distance(new_destination->destination_coordinates, fish->top_left)) / fish->speed);
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
    printf("origin: %dx%d", origin->destination_coordinates.x, origin->destination_coordinates.y);
    for (int i = 0; intersections[i].x != -1 && i < len_views_destination + len_views_origin + 1; i++) {
        if (intersections[i].x == origin->destination_coordinates.x && intersections[i].y == origin->destination_coordinates.y) {
            // if the intersection is the same as the one before, we don't add it
            continue;
        } else if (intersections[i].x < 0 || intersections[i].y < 0) {
            // if the intersection is not in the aquarium, we don't add it
            continue;
        }
        printf(" --> %dx%d", intersections[i].x, intersections[i].y);
        new_destination = malloc(sizeof(struct fish_destination));
        new_destination->destination_coordinates.x = intersections[i].x;
        new_destination->destination_coordinates.y = intersections[i].y;
        new_destination->time_at_destination = origin->time_at_destination + (time_t)((distance(new_destination->destination_coordinates, origin->destination_coordinates)) / fish->speed);
        STAILQ_INSERT_AFTER(&fish->destinations_queue, origin, new_destination, next);
        origin = new_destination; // update origin
    }
    printf(" --> %dx%d\n", destination->destination_coordinates.x, destination->destination_coordinates.y);
    free(intersections);
    return OK;
}

struct coordinates *get_instersections_btw_trajectory_and_views(struct view **views, struct coordinates *p1, struct coordinates *p2) {
    printf("from %dx%d to %dx%d\n", p1->x, p1->y, p2->x, p2->y);
    int len_views;
    for (len_views = 0; views[len_views] != NULL; len_views++) {
    }
    struct coordinates *intersections = malloc(sizeof(struct coordinates) * len_views);

    if (p2->x == p1->x) {
        for (int i = 0; i < len_views; i++) {
            intersections[i].x = p1->x;
            intersections[i].y = views[i]->top_left.y;
        }
        return intersections;
    }
    float slope_trajectory = (float)(p2->y - p1->y) / (float)(p2->x - p1->x); // a in y = ax+ b
    float y_intercept_trajectory = p1->y - slope_trajectory * p1->x; // b in y = ax+ b
    for (int i = 0; i < len_views; i++) {
        float top_side_intersection = (views[i]->top_left.y - y_intercept_trajectory) / slope_trajectory;
        float bottom_side_intersection = (views[i]->top_left.y + views[i]->height - y_intercept_trajectory) / slope_trajectory;
        float left_side_intersection = slope_trajectory * views[i]->top_left.x + y_intercept_trajectory;
        float right_side_intersection = slope_trajectory * (views[i]->top_left.x + views[i]->width) + y_intercept_trajectory;
        printf("top: %f, bottom: %f, left: %f, right: %f\n", top_side_intersection, bottom_side_intersection, left_side_intersection, right_side_intersection);
        printf(" ==> top: %dx%d\n", (int)top_side_intersection, views[i]->top_left.y);
        printf(" ==> bottom: %dx%d\n", (int)bottom_side_intersection, views[i]->top_left.y + views[i]->height);
        printf(" ==> left: %dx%d\n", views[i]->top_left.x, (int)left_side_intersection);
        printf(" ==> right: %dx%d\n", views[i]->top_left.x + views[i]->width, (int)right_side_intersection);
        /* Coordinates are cast to int ==> rounds down */
        if (top_side_intersection <= views[i]->top_left.x + views[i]->width && top_side_intersection >= views[i]->top_left.x && top_side_intersection >= p1->x && top_side_intersection <= p2->x) {
            intersections[i].x = top_side_intersection;
            intersections[i].y = views[i]->top_left.y;
            i++;
        }
        if (bottom_side_intersection <= views[i]->top_left.x + views[i]->width && bottom_side_intersection >= views[i]->top_left.x && bottom_side_intersection >= p1->x && bottom_side_intersection <= p2->x) {
            intersections[i].x = bottom_side_intersection;
            intersections[i].y = views[i]->top_left.y + views[i]->height;
            i++;
        }
        if (left_side_intersection <= views[i]->top_left.y + views[i]->height && left_side_intersection >= views[i]->top_left.y && left_side_intersection >= p1->y && left_side_intersection <= p2->y) {
            intersections[i].x = views[i]->top_left.x;
            intersections[i].y = left_side_intersection;
            i++;
        }
        if (right_side_intersection <= views[i]->top_left.y + views[i]->height && right_side_intersection >= views[i]->top_left.y && right_side_intersection >= p1->y && right_side_intersection <= p2->y) {
            intersections[i].x = views[i]->top_left.x + views[i]->width;
            intersections[i].y = right_side_intersection;
            i++;
        }
        // else {
        //     intersections[i].x = -1;
        //     intersections[i].y = -1;
        // }
    }
    printf("intersections before sort:\n");
    for (int i = 0; i < len_views; i++) {
        printf(" ==> %dx%d\n", intersections[i].x, intersections[i].y);
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
    // printf("intersections after sort:\n");
    // for (int i = 0; i < len_views; i++) {
    //     printf(" ==> %dx%d\n", intersections[i].x, intersections[i].y);
    // }
    // printf("\n\n");

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