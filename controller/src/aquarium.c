#include "aquarium.h"
#include "utils.h"

int add_fish(struct aquarium *a, struct fish *f) {
    // if the aquarium is empty, add the fish
    if (a->fishes == NULL) {
        a->fishes = f;
        return EXIT_SUCCESS;
    }
    // if the aquarium is not empty, check if the fish is already in the aquarium
    struct fish *current = a->fishes;
    while (current->next != NULL) {
        current = current->next;
        if (strcmp(current->name, f->name) == 0) {
            return EXIT_FAILURE;
        }
    }
    // if the fish is not in the aquarium, add it to the end of the list
    current->next = f;
    return EXIT_SUCCESS;
}

int remove_fish(struct aquarium *a, struct fish *f) {
    // if the aquarium is empty, return failure
    if (a->fishes == NULL) {
        return EXIT_FAILURE;
    }
    // if the aquarium is not empty, check if the fish is in the aquarium
    struct fish *current = a->fishes;
    struct fish *previous = NULL;
    while (current->next != NULL) {
        if (strcmp(current->name, f->name) == 0) {
            // if the fish is in the aquarium, remove it
            if (previous == NULL) {
                a->fishes = current->next;
            } else {
                previous->next = current->next;
            }
            return EXIT_SUCCESS;
        }
        previous = current;
        current = current->next;
    }
    // if the fish is not in the aquarium, return failure
    return EXIT_FAILURE;
}

struct fish *create_fish(const char *name, struct coordinates top_left, int height, int width, enum movement_pattern mvt) {
    // create a new fish
    struct fish *f = malloc(sizeof(struct fish));
    exit_if(f == NULL, "malloc failed");
    f->name = name;
    f->top_left = top_left;
    f->height = height;
    f->width = width;
    f->movement_pattern = mvt;
    f->status = NOT_STARTED;
    f->next = NULL;
    return f;
}

struct fish *get_fish(struct aquarium *a, const char *name) {
    // if the aquarium is empty, return NULL
    if (a->fishes == NULL) {
        return NULL;
    }
    // if the aquarium is not empty, check if the fish is in the aquarium
    struct fish *current = a->fishes;
    while (current->next != NULL) {
        if (strcmp(current->name, name) == 0) {
            // if the fish is in the aquarium, return it
            return current;
        }
        current = current->next;
    }
    // if the fish is not in the aquarium, return NULL
    return NULL;
}

struct view *create_view(const char *name, struct coordinates top_left, int height, int width) {
    // create a new view
    struct view *v = malloc(sizeof(struct view));
    exit_if(v == NULL, "malloc failed");
    v->top_left = top_left;
    v->height = height;
    v->width = width;
    v->name = name;
    v->next = NULL;
    return v;
}

int add_view(struct aquarium *a, struct view *v) {
    // if the aquarium is empty, add the view
    if (a->views == NULL) {
        a->views = v;
        return EXIT_SUCCESS;
    }
    // if the aquarium is not empty, check if the view is already in the aquarium
    struct view *current = a->views;
    while (current->next != NULL) {
        current = current->next;
        if (strcmp(current->name, v->name) == 0) {
            return EXIT_FAILURE;
        }
    }
    // if the view is not in the aquarium, add it to the end of the list
    current->next = v;
    return EXIT_SUCCESS;
}

int remove_view(struct aquarium *a, struct view *v) {
    // if the aquarium is empty, return failure
    if (a->views == NULL) {
        return EXIT_FAILURE;
    }
    // if the aquarium is not empty, check if the view is in the aquarium
    struct view *current = a->views;
    struct view *previous = NULL;
    while (current->next != NULL) {
        if (strcmp(current->name, v->name) == 0) {
            // if the view is in the aquarium, remove it
            if (previous == NULL) {
                a->views = current->next;
            } else {
                previous->next = current->next;
            }
            return EXIT_SUCCESS;
        }
        previous = current;
        current = current->next;
    }
    // if the view is not in the aquarium, return failure
    return EXIT_FAILURE;
}

struct view *get_view(struct aquarium *a, const char *name) {
    // if the aquarium is empty, return NULL
    if (a->views == NULL) {
        return NULL;
    }
    // if the aquarium is not empty, check if the view is in the aquarium
    struct view *current = a->views;
    while (current->next != NULL) {
        if (strcmp(current->name, name) == 0) {
            // if the view is in the aquarium, return it
            return current;
        }
        current = current->next;
    }
    // if the view is not in the aquarium, return NULL
    return NULL;
}

struct aquarium *create_aquarium(int height, int width) {
    // create a new aquarium
    struct aquarium *a = malloc(sizeof(struct aquarium));
    exit_if(a == NULL, "malloc failed");
    a->fishes = NULL;
    a->views = NULL;
    return a;
}

struct fish *get_fishes(struct aquarium *a) {
    return a->fishes;
}

int len_views(struct aquarium *a) {
    int nb_views;

    // if the aquarium is empty
    if (a->views == NULL) {
        return 0;
    }
    // if the aquarium is not empty
    struct view *current = a->views;
    while (current->next != NULL) {
        current = current->next;
        nb_views++;
    }

    return nb_views;
}

void show_aquarium(struct aquarium *a, FILE *f) {
    fprintf(f, "%dx%d\n", a->width, a->height);

    struct view *current_view = a->views;
    struct view *next_view = current_view->next;
    for (int i = 0; i < len_views(a); i++) {
        fprintf(f, "%s %dx%d+%d+%d\n", current_view->name, current_view->top_left.x, current_view->top_left.y, current_view->width, current_view->height);
        current_view = next_view;
        next_view = current_view->next;
    }
}

int len_fishes(struct aquarium *a) {
    int nb_fishes;

    // if the aquarium is empty
    if (a->fishes == NULL) {
        return 0;
    }
    // if the aquarium is not empty
    struct fish *current = a->fishes;
    while (current->next != NULL) {
        current = current->next;
        nb_fishes++;
    }

    return nb_fishes;
}

void save_aquarium(struct aquarium *a, const char *name) {
    FILE *f = fopen(name, "w");
    show_aquarium(a, f);
    fclose(f);
}