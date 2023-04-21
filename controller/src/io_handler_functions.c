#include "io_handler_functions.h"
#include "socket_aquarium.h"

void hello_handler(struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    struct view *view;
    if (parser->size == 3) {
        view = get_view(aquarium, parser->tab[2]);
        if (view != NULL && view->socket_fd == -1) {
            view->socket_fd = socket_fd;
            dprintf(socket_fd, "greeting %s\n", view->name);
        }
    } else {
        view = get_first_free_view_socket(aquarium);
        if (view == NULL) {
            dprintf(socket_fd, "no greeting\n");
        } else {
            view->socket_fd = socket_fd;
            dprintf(socket_fd, "greeting %s\n", view->name);
        }
    }
}

void get_fishes_handler(__attribute__((unused))struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    struct view *view = get_view_from_socket(aquarium, socket_fd);
    struct fish **fishes_in_view = get_fishes_in_view(aquarium, view, 1);
    dprintf(socket_fd, "list");
    int iter = 0;
    while (fishes_in_view[iter] != NULL) {
        dprintf(socket_fd, " [%s at %dx%d,%dx%d,%ld]", fishes_in_view[iter]->name, x_coordinate_to_percentage(view, fishes_in_view[iter]->top_left.x), y_coordinate_to_percentage(view, fishes_in_view[iter]->top_left.y), x_coordinate_to_percentage(view, fishes_in_view[iter]->destination.x), y_coordinate_to_percentage(view, fishes_in_view[iter]->destination.y), fishes_in_view[iter]->time_to_destination - time(NULL));
        iter++;
    }
    dprintf(socket_fd, "\n");
    free(fishes_in_view);
}

void ping_handler(struct parse *parser, int socket_fd, __attribute__((unused))struct aquarium *aquarium) {
    dprintf(socket_fd, "pong %s\n", parser->tab[1]);
}


void add_fish_handler(struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    // à changer une fois que Fatima aura corriger le parseur (stocke trop d'informations inutiles)
    if (get_fish_from_name(aquarium, parser->tab[1]) != NULL) {
        dprintf(socket_fd, "NOK\n");
    } else {
        add_fish(aquarium, create_fish(parser->tab[1], (struct coordinates) { atoi(parser->tab[3]), atoi(parser->tab[4]) }, atoi(parser->tab[5]), atoi(parser->tab[6]), RANDOMWAYPOINT));
        dprintf(socket_fd, "OK\n");
    }
}

void del_fish_handler(struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    if (remove_fish(aquarium, get_fish_from_name(aquarium, parser->tab[1]))) {
        dprintf(socket_fd, "OK\n");
    } else {
        dprintf(socket_fd, "NOK\n");
    }
}

void start_fish_handler(struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    if (start_fish(aquarium, parser->tab[1])) {
        dprintf(socket_fd, "OK\n");
    } else {
        dprintf(socket_fd, "NOK\n");
    }
}

void log_out_handler(__attribute__((unused))struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    struct view *view = get_view_from_socket(aquarium, socket_fd);
    view->socket_fd = -1;
    dprintf(socket_fd, "bye\n");
}

void status_handler(__attribute__((unused))struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    dprintf(socket_fd, "OK: Connected to controller, %d fishes found", len_fishes(aquarium));
    struct fish **fishes_in_view = get_fishes_in_view(aquarium, get_view_from_socket(aquarium, socket_fd), 0); // 0 = false
    int iter = 0;
    while (fishes_in_view[iter] != NULL) {
        dprintf(socket_fd, "\tFish %s at %dx%d,%dx%d %s", fishes_in_view[iter]->name, fishes_in_view[iter]->top_left.x, fishes_in_view[iter]->top_left.y, fishes_in_view[iter]->width, fishes_in_view[iter]->height, fishes_in_view[iter]->status == STARTED ? "started" : "notStarted");
        iter++;
    }
    dprintf(socket_fd, "\n");
    free(fishes_in_view);
}

void ls_handler(__attribute__((unused))struct parse *parser, __attribute__((unused))int socket_fd, __attribute__((unused))struct aquarium *aquarium) {
    ;
}

void get_fishes_continuously_handler(__attribute__((unused))struct parse *parser, __attribute__((unused))int socket_fd, __attribute__((unused))struct aquarium *aquarium) {
    ;
}