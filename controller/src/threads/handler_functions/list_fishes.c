#include "list_fishes.h"

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
    log_message(log, LOG_INFO, "list");
    while (fishes_in_view[iter] != NULL) {
        destination = STAILQ_FIRST(&fishes_in_view[iter]->destinations_queue);

        // searching for destination to send to view
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
        log_message(log, LOG_INFO, " [%s at %dx%d,%dx%d,%ld]", fishes_in_view[iter]->name, x_coordinate_to_percentage(view, destination->destination_coordinates.x), y_coordinate_to_percentage(view, destination->destination_coordinates.y), fishes_in_view[iter]->width, fishes_in_view[iter]->height, get_seconds_to_get_to_time_in_milliseconds(destination->time_at_destination));
        mark_destination_as_sent(view->name, destination);
        log_message(log, LOG_INFO, "Destination %dx%d marked as sent for view %s", destination->destination_coordinates.x, destination->destination_coordinates.y, view->name);

        iter++;
    }
    if (dprintf(socket_fd, "\n") < 0) {
        log_message(log, LOG_ERROR, "Could not write on the socket %d", socket_fd);
    }
    log_message(log, LOG_INFO, "\n");
}