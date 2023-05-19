#include "io_handler_functions.h"
#include "../../communication/socket_aquarium.h"
#include "../../aquarium/fish.h"
#include "../../utils.h"
#include "../handle_fishes_continuously_thread.h"


int handle_error(FILE *log, struct parse *parser, int socket_fd) {
    if (strcmp(parser->status, "OK\n") != 0) {
        dprintf(socket_fd, "%s", parser->status);
        fprintf(log, "%s", parser->status);
        fflush(log);
        return OK;
    }
    return NOK;
}

void print_list_fish_for_client(FILE *log, struct fish **fishes_in_view, struct view *view, int socket_fd, int bool_get_next_destination) {
    if (fishes_in_view[0] == NULL) {
        fprintf(log, "Error: no fish in view\n");
        fflush(log);
        return;
    }
    if (view == NULL) {
        fprintf(log, "Error: view is NULL\n");
        fflush(log);
        return;
    }

    int iter = 0;
    struct fish_destination *destination;
    dprintf(socket_fd, "list");
    while (fishes_in_view[iter] != NULL) {
        if (bool_get_next_destination) {
            if (len_movements_queue(fishes_in_view[iter]) < 2) {
                fprintf(log, "Error: fish %s has no next destination\n", fishes_in_view[iter]->name);
                iter++;
                continue;
            }
            destination = STAILQ_NEXT(STAILQ_FIRST(&fishes_in_view[iter]->destinations_queue), next);
        } else {
            destination = STAILQ_FIRST(&fishes_in_view[iter]->destinations_queue);
        }
        if (destination == NULL) {
            dprintf(socket_fd, " [%s at %dx%d,%dx%d,%d]", fishes_in_view[iter]->name, x_coordinate_to_percentage(view, fishes_in_view[iter]->top_left.x), y_coordinate_to_percentage(view, fishes_in_view[iter]->top_left.y), fishes_in_view[iter]->width, fishes_in_view[iter]->height, 0);
            iter++;
            continue;
        }
        dprintf(socket_fd, " [%s at %dx%d,%dx%d,%ld]", fishes_in_view[iter]->name, x_coordinate_to_percentage(view, destination->destination_coordinates.x), y_coordinate_to_percentage(view, destination->destination_coordinates.y), fishes_in_view[iter]->width, fishes_in_view[iter]->height, destination->time_at_destination - time(NULL));
        iter++;
    }
    dprintf(socket_fd, "\n");
}

void hello_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    struct view *view;
    if (parser->size == 1) {
        view = get_view(aquarium, parser->arguments[0]);
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

void get_fishes_handler(FILE *log, __attribute__((unused))struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    struct view *view = get_view_from_socket(aquarium, socket_fd);
    struct fish **fishes_in_view = get_fishes_with_destination_in_view(aquarium, view, 1);
    print_list_fish_for_client(log, fishes_in_view, view, socket_fd, 0);
    free_fishes_array(fishes_in_view, view);
}

void ping_handler(FILE *log, struct parse *parser, int socket_fd, __attribute__((unused))struct aquarium *aquarium) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    dprintf(socket_fd, "pong %s\n", parser->arguments[0]);
}


void add_fish_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    if (get_fish_from_name(aquarium, parser->arguments[0]) != NULL) {
        dprintf(socket_fd, "NOK\n");
        return;
    }
    struct view *view = get_view_from_socket(aquarium, socket_fd);
    if (!add_fish(aquarium, create_fish(parser->arguments[0], (struct coordinates) { percentage_to_x_coordinate(view, atoi(parser->arguments[1])), percentage_to_y_coordinate(view, atoi(parser->arguments[2])) }, atoi(parser->arguments[3]), atoi(parser->arguments[4]), RANDOMWAYPOINT))) {
        fprintf(log, "Error: fish %s at %dx%d in view not added\n", parser->arguments[0], atoi(parser->arguments[1]), atoi(parser->arguments[2]));
        dprintf(socket_fd, "NOK\n");
        return;
    }
    struct fish *fish_just_created = get_fish_from_name(aquarium, parser->arguments[0]);
    fprintf(log, "fish %s at %dx%d in aquarium added\n", fish_just_created->name, fish_just_created->top_left.x, fish_just_created->top_left.y);
    dprintf(socket_fd, "OK\n");
}

void del_fish_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    struct fish *fish_to_remove = get_fish_from_name(aquarium, parser->arguments[0]);
    if (fish_to_remove == NULL) {
        fprintf(log, "Error: fish %s not found\n", parser->arguments[0]);
        dprintf(socket_fd, "NOK\n");
        return;
    }
    if (remove_fish(aquarium, fish_to_remove)) {
        dprintf(socket_fd, "OK\n");
        return;
    }
    dprintf(socket_fd, "NOK\n");
}

void start_fish_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    fprintf(log, "start fish %s\n", parser->arguments[0]);
    fflush(log);
    if (start_fish(aquarium, parser->arguments[0])) {
        dprintf(socket_fd, "OK\n");
        return;
    }
    dprintf(socket_fd, "NOK\n");
}

void log_out_handler(FILE *log, struct parse *parser, int *socket_fd, struct aquarium *aquarium) {
    if (handle_error(log, parser, *socket_fd)) {
        return;
    }
    struct view *view = get_view_from_socket(aquarium, *socket_fd);
    dprintf(*socket_fd, "bye\n");
    close(*socket_fd);
    view->socket_fd = -1;
    *socket_fd = -1;
}

// void status_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium) {
//     if (handle_error(log, parser, socket_fd)) {
//         return;
//     }
//     dprintf(socket_fd, "OK: Connected to controller, %d fishes found", len_fishes(aquarium));
//     struct fish **fishes_in_view = get_fishes_with_destination_in_view(aquarium, get_view_from_socket(aquarium, socket_fd), 0); // 0 = false
//     if (fishes_in_view == NULL) {
//         fprintf(log, "Error: no fishes in view\n");
//         fflush(log);
//         dprintf(socket_fd, "\n");
//         return;
//     }
//     int iter = 0;
//     while (fishes_in_view[iter] != NULL) {
//         dprintf(socket_fd, "\tFish %s at %dx%d,%dx%d %s", fishes_in_view[iter]->name, fishes_in_view[iter]->top_left.x, fishes_in_view[iter]->top_left.y, fishes_in_view[iter]->width, fishes_in_view[iter]->height, fishes_in_view[iter]->status == STARTED ? "started" : "notStarted");
//         iter++;
//     }
//     dprintf(socket_fd, "\n");
//     free_fishes_array(fishes_in_view, view);
// }

void ls_handler(FILE *log, struct parse *parser, __attribute__((unused))int socket_fd, __attribute__((unused))struct aquarium *aquarium) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    struct view *view = get_view_from_socket(aquarium, socket_fd);
    struct fish **fishes_in_view = get_fishes_with_destination_in_view(aquarium, view, 1);
    print_list_fish_for_client(log, fishes_in_view, view, socket_fd, 0);
    print_list_fish_for_client(log, fishes_in_view, view, socket_fd, 1);
    // dprintf(socket_fd, "\n"); // to signal end of lists to client
    free_fishes_array(fishes_in_view, view);
}

void get_fishes_continuously_handler(FILE *log, struct parse *parser, int socket_fd, pthread_t *handle_fishes_continuously_thread) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    struct handle_fishes_continuously_parameters *parameters = malloc(sizeof(struct handle_fishes_continuously_parameters));
    parameters->socket_fd = socket_fd;
    pthread_mutex_unlock(&aquarium_mutex);
    pthread_create(handle_fishes_continuously_thread, NULL, (void *(*)(void *))get_fishes_continuously, parameters);
    pthread_mutex_lock(&aquarium_mutex);
}
