#include "aquarium.h"


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

void deep_copy_aquarium(struct aquarium *dest, struct aquarium *src) {
    dest->width = src->width;
    dest->height = src->height;
    dest->fishes = src->fishes;
    dest->views = src->views;
}

void save_aquarium(struct aquarium *aquarium, char *name) {
    FILE *f = fopen(name, "w");
    show_aquarium(aquarium, f);
    fclose(f);
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
            free_fish(current_fish);
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
            free_view(current_view);
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