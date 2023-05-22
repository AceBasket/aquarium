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
    char file_name[40];
    sprintf(file_name, "log_handle_fishes_continuously_%d.log", socket_fd);
    FILE *log = fopen(file_name, "w");
    exit_if(log == NULL, "fopen failed");
    log_message(log, LOG_INFO, "===== get_fishes_continuously() =====");

    if (signal(SIGPIPE, sigpipe_handler) == SIG_ERR) {
        log_message(log, LOG_ERROR, "The signal handler could not be changed");
    }

    pthread_mutex_lock(&aquarium_mutex);
    struct view *view = get_view_from_socket(aquarium, socket_fd);
    fprintf(log, "View: %s\n", view->name);
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
            /* Checking that we can initialize our search for the minimum time at destination */
            if (STAILQ_EMPTY(&fishes_in_view[0]->destinations_queue)) {
                pthread_mutex_unlock(&aquarium_mutex);
                log_message(log, LOG_WARNING, "No destination in the queue");
                pthread_mutex_lock(&aquarium_mutex);
            } else {
                /* Finding the minimum time at destination */
                minimum_time_to_destination = STAILQ_FIRST(&fishes_in_view[0]->destinations_queue)->time_at_destination;

                int iter = 0;
                fprintf(log, "Fishes in view:\n");
                fflush(log);
                while (fishes_in_view[iter] != NULL) {
                    struct fish_destination *destination = STAILQ_FIRST(&fishes_in_view[iter]->destinations_queue);
                    if (destination == NULL) {
                        iter++;
                        continue;
                    }
                    if (destination->time_at_destination < minimum_time_to_destination) {
                        minimum_time_to_destination = destination->time_at_destination;
                    }
                    fprintf(log, "Fish %s:\n", fishes_in_view[iter]->name);
                    fflush(log);
                    debug_destinations_queue(log, fishes_in_view[iter]);

                    /* Mark destination as sent (we will send them in next instruction) */
                    // mark_destination_as_sent(view->name, destination);
                    iter++;

                }
                list_fishes_for_client(log, fishes_in_view, view, socket_fd);



            }
        }
        free_fishes_array(fishes_in_view, view);

        pthread_mutex_unlock(&aquarium_mutex);
        // if (minimum_time_to_destination > time(NULL)) {
        //     usleep((minimum_time_to_destination - time(NULL)) * 1000000 - 500000); // - 500000 to be sure to not miss the time
        // } else { // in case there is no fish in the view
        //     sleep(1);
        // }
        // usleep(800000);
        sleep(1);
        pthread_mutex_lock(&terminate_threads_mutex);
    }
    pthread_mutex_unlock(&terminate_threads_mutex);
    free(parameters);
    log_message(log, LOG_INFO, "===== get_fishes_continuously() terminated =====");
    fclose(log);
    return NULL;
}