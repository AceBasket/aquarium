#include "handle_fishes_continuously_thread.h"
#include "../communication/socket_aquarium.h"
#include "handler_functions/io_handler_functions.h"
#include "../aquarium/fish.h"
#include "../aquarium/view.h"
#include "../aquarium/aquarium.h"
#include "../utils.h"
#include <time.h>
#include <unistd.h>

void *get_fishes_continuously(void *parameters) {
    int socket_fd = ((struct handle_fishes_continuously_parameters *)parameters)->socket_fd;
    FILE *log = fopen("log_handle_fishes_continuously.log", "w");
    log_message(log, LOG_INFO, "===== get_fishes_continuously() =====");

    if (signal(SIGPIPE, sigpipe_handler) == SIG_ERR) {
        log_message(log, LOG_ERROR, "The signal handler could not be changed");
    }

    pthread_mutex_lock(&aquarium_mutex);
    struct view *view = get_view_from_socket(aquarium, socket_fd);
    pthread_mutex_unlock(&aquarium_mutex);
    struct fish **fishes_in_view;
    time_t minimum_time_to_destination = 0;

    pthread_mutex_lock(&terminate_threads_mutex);
    while (terminate_threads == NOK) {
        pthread_mutex_unlock(&terminate_threads_mutex);

        pthread_mutex_lock(&aquarium_mutex);
        fishes_in_view = get_fishes_with_destination_in_view(aquarium, view, 0); // 0 = false
        if (fishes_in_view[0] == NULL) {
            pthread_mutex_unlock(&aquarium_mutex);
            log_message(log, LOG_WARNING, "No fish in the view");
            pthread_mutex_lock(&aquarium_mutex);
        } else {
            if (STAILQ_EMPTY(&fishes_in_view[0]->destinations_queue)) {
                pthread_mutex_unlock(&aquarium_mutex);
                log_message(log, LOG_WARNING, "No destination in the queue");
                pthread_mutex_lock(&aquarium_mutex);
            } else {
                minimum_time_to_destination = STAILQ_FIRST(&fishes_in_view[0]->destinations_queue)->time_at_destination;
                int iter = 1;
                while (fishes_in_view[iter] != NULL) {
                    struct fish_destination *destination = STAILQ_FIRST(&fishes_in_view[iter]->destinations_queue);
                    if (destination == NULL) {
                        iter++;
                        continue;
                    }
                    if (destination->time_at_destination < minimum_time_to_destination) {
                        minimum_time_to_destination = destination->time_at_destination;
                    }
                    iter++;
                }
                debug_destinations_queue(log, fishes_in_view[0]);
                print_list_fish_for_client(log, fishes_in_view, view, socket_fd, 0); // 0 = false
            }
        }
        free_fishes_array(fishes_in_view, view);

        pthread_mutex_unlock(&aquarium_mutex);
        if (minimum_time_to_destination > time(NULL)) {
            if (sleep(minimum_time_to_destination - time(NULL)) != 0) {
                log_message(log, LOG_ERROR, "Sleep() interrupted");
            }
        } else {
            if (sleep(1) != 0) {
                log_message(log, LOG_ERROR, "Sleep() interrupted");
            }
        }
        pthread_mutex_lock(&terminate_threads_mutex);
    }
    pthread_mutex_unlock(&terminate_threads_mutex);
    free(parameters);
    log_message(log, LOG_INFO, "===== get_fishes_continuously() terminated =====");
    fclose(log);
    return NULL;
}