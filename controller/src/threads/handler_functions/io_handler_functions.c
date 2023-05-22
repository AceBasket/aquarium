#include "io_handler_functions.h"
#include "../../communication/socket_aquarium.h"
#include "../../aquarium/fish.h"
#include "../../utils.h"
#include "../handle_fishes_continuously_thread.h"


int handle_error(FILE *log, struct parse *parser, int socket_fd) {
    if (strcmp(parser->status, "OK\n") != 0) {
        if (dprintf(socket_fd, "%s", parser->status) < 0) {
            log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
        }
        log_message(log, LOG_INFO, "%s", parser->status);
        return OK;
    }
    return NOK;
}



/**
 * @brief Prints one destination per fish on the socket. If the first destination was already sent, print the next unsent one
 *
 */
void list_fishes_for_client(FILE *log, struct fish **fishes_in_view, struct view *view, int socket_fd) {
    if (fishes_in_view[0] == NULL) {
        log_message(log, LOG_WARNING, "No fish in view");
        return;
    }
    if (view == NULL) {
        log_message(log, LOG_WARNING, "View is NULL");
        return;
    }

    int iter = 0;
    struct fish_destination *destination;
    if (dprintf(socket_fd, "list") < 0) {
        log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
    }
    fprintf(log, "list\n");
    while (fishes_in_view[iter] != NULL) {
        destination = STAILQ_FIRST(&fishes_in_view[iter]->destinations_queue);

        /* Searching for destination to send to view */
        while (destination_sent_to_view(view->name, destination) == OK) {
            destination = STAILQ_NEXT(destination, next);
        }

        if (destination == NULL) {
            // skip fish
            iter++;
            continue;
        }
        if (dprintf(socket_fd, " [%s at %dx%d,%dx%d,%ld]", fishes_in_view[iter]->name, x_coordinate_to_percentage(view, destination->destination_coordinates.x), y_coordinate_to_percentage(view, destination->destination_coordinates.y), fishes_in_view[iter]->width, fishes_in_view[iter]->height, get_seconds_to_get_to_time_in_milliseconds(destination->time_at_destination)) < 0) {
            log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
        }
        fprintf(log, " [%s at %dx%d,%dx%d,%ld]", fishes_in_view[iter]->name, x_coordinate_to_percentage(view, destination->destination_coordinates.x), y_coordinate_to_percentage(view, destination->destination_coordinates.y), fishes_in_view[iter]->width, fishes_in_view[iter]->height, get_seconds_to_get_to_time_in_milliseconds(destination->time_at_destination));

        mark_destination_as_sent(view->name, destination);
        fprintf(log, "Destination %dx%d marked as sent for view %s\n", destination->destination_coordinates.x, destination->destination_coordinates.y, view->name);
        fflush(log);

        iter++;
    }
    if (dprintf(socket_fd, "\n") < 0) {
        log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
    }
    fprintf(log, "\n");
    fflush(log);
}

void hello_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    struct view *view = NULL;
    if (parser->size == 1) {
        view = get_view(aquarium, parser->arguments[0]);
        if (view != NULL && view->socket_fd == -1) {
            view->socket_fd = socket_fd;
            if (dprintf(socket_fd, "greeting %s\n", view->name) < 0) {
                log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
            }
            log_message(log, LOG_INFO, "View %s connected", view->name);
            return;
        }
    }
    if ((view != NULL && view->socket_fd != -1) || view == NULL) {
        view = get_first_free_view_socket(aquarium);
        if (view == NULL) {
            if (dprintf(socket_fd, "no greeting\n") < 0) {
                log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
            }
            log_message(log, LOG_WARNING, "View not connected");
        } else {
            view->socket_fd = socket_fd;
            if (dprintf(socket_fd, "greeting %s\n", view->name) < 0) {
                log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
            }
            log_message(log, LOG_INFO, "View %s connected", view->name);
        }
    }
}

void get_fishes_handler(FILE *log, __attribute__((unused))struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    struct view *view = get_view_from_socket(aquarium, socket_fd);
    struct fish **fishes_in_view = get_fishes_with_destination_in_view(aquarium, view, 1);
    list_fishes_for_client(log, fishes_in_view, view, socket_fd);
    free_fishes_array(fishes_in_view, view);
}

void ping_handler(FILE *log, struct parse *parser, int socket_fd, __attribute__((unused))struct aquarium *aquarium) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    if (dprintf(socket_fd, "pong %s\n", parser->arguments[0]) < 0) {
        log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
    }
}


void add_fish_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    if (get_fish_from_name(aquarium, parser->arguments[0]) != NULL) {
        if (dprintf(socket_fd, "NOK\n") < 0) {
            log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
        }
        return;
    }
    struct view *view = get_view_from_socket(aquarium, socket_fd);
    if (!add_fish(aquarium, create_fish(parser->arguments[0], (struct coordinates) { percentage_to_x_coordinate(view, atoi(parser->arguments[1])), percentage_to_y_coordinate(view, atoi(parser->arguments[2])) }, atoi(parser->arguments[3]), atoi(parser->arguments[4]), RANDOMWAYPOINT))) {
        log_message(log, LOG_WARNING, "Fish %s at %dx%d in view not added", parser->arguments[0], atoi(parser->arguments[1]), atoi(parser->arguments[2]));
        if (dprintf(socket_fd, "NOK\n") < 0) {
            log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
        }
        return;
    }
    struct fish *fish_just_created = get_fish_from_name(aquarium, parser->arguments[0]);
    log_message(log, LOG_INFO, "Fish %s at %dx%d in aquarium added", fish_just_created->name, fish_just_created->top_left.x, fish_just_created->top_left.y);
    if (dprintf(socket_fd, "OK\n") < 0) {
        log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
    }
}

void del_fish_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    struct fish *fish_to_remove = get_fish_from_name(aquarium, parser->arguments[0]);
    if (fish_to_remove == NULL) {
        log_message(log, LOG_WARNING, "Fish %s not found", parser->arguments[0]);
        if (dprintf(socket_fd, "NOK\n") < 0) {
            log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
        }
        return;
    }
    if (remove_fish(aquarium, fish_to_remove)) {
        if (dprintf(socket_fd, "OK\n") < 0) {
            log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
        }
        return;
    }
    if (dprintf(socket_fd, "NOK\n") < 0) {
        log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
    }
}

void start_fish_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    log_message(log, LOG_INFO, "Start fish %s", parser->arguments[0]);
    if (start_fish(aquarium, parser->arguments[0])) {
        if (dprintf(socket_fd, "OK\n") < 0) {
            log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
        }
        return;
    }
    if (dprintf(socket_fd, "NOK\n") < 0) {
        log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
    }
}

void log_out_handler(FILE *log, struct parse *parser, int *socket_fd, struct aquarium *aquarium) {
    if (handle_error(log, parser, *socket_fd)) {
        return;
    }
    struct view *view = get_view_from_socket(aquarium, *socket_fd);
    if (dprintf(*socket_fd, "bye\n") < 0) {
        log_message(log, LOG_ERROR, "Could not write on the socket %d", *socket_fd);
    }
    if (close(*socket_fd) != 0) {
        log_message(log, LOG_ERROR, "The socket %d could not be closed", *socket_fd);
    }
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
    list_fishes_for_client(log, fishes_in_view, view, socket_fd);
    // dprintf(socket_fd, "\n"); // to signal end of lists to client
    free_fishes_array(fishes_in_view, view);
}

void get_fishes_continuously_handler(FILE *log, struct parse *parser, int socket_fd, pthread_t *handle_fishes_continuously_thread) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    struct handle_fishes_continuously_parameters *parameters = malloc(sizeof(struct handle_fishes_continuously_parameters));
    if (parameters == NULL) {
        log_message(log, LOG_ERROR, "The memory could not be allocated");
    }
    parameters->socket_fd = socket_fd;
    pthread_mutex_unlock(&aquarium_mutex);
    if (pthread_create(handle_fishes_continuously_thread, NULL, (void *(*)(void *))get_fishes_continuously, parameters) != 0) {
        log_message(log, LOG_FATAL_ERROR, "On handle fishes continuously thread creation");
    }
    pthread_mutex_lock(&aquarium_mutex);
}
