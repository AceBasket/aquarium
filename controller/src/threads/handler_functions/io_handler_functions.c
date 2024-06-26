#include "io_handler_functions.h"

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

void ls_handler(FILE *log, struct parse *parser, __attribute__((unused))int socket_fd, __attribute__((unused))struct aquarium *aquarium) {
    if (handle_error(log, parser, socket_fd)) {
        return;
    }
    struct view *view = get_view_from_socket(aquarium, socket_fd);
    struct fish **fishes_in_view = get_fishes_with_destination_in_view(aquarium, view, 1);
    list_fishes_for_client(log, fishes_in_view, view, socket_fd);
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
