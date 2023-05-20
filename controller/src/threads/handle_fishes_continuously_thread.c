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

    signal(SIGPIPE, sigpipe_handler);

    int socket_fd = ((struct handle_fishes_continuously_parameters *)parameters)->socket_fd;
    int file_name_len = strlen("log_handle_fishes_continuously.log") + 1 + 1;
    char file_name[40] = {};
    strcpy(file_name, "log_handle_fishes_continuously.log");
    file_name[file_name_len - 2] = '0' + socket_fd;
    file_name[file_name_len - 1] = '\0';
    FILE *log = fopen(file_name, "w");
    exit_if(log == NULL, "fopen failed");
    fprintf(log, "===== get_fishes_continuously() =====\n");
    fflush(log);

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
            fprintf(log, "Error: no fishes in view\n");
            fflush(log);
            pthread_mutex_lock(&aquarium_mutex);
        } else {
            /* Checking that we can initialize our search for the minimum time at destination */
            if (STAILQ_EMPTY(&fishes_in_view[0]->destinations_queue)) {
                pthread_mutex_unlock(&aquarium_mutex);
                fprintf(log, "Error: no destinations in queue\n");
                fflush(log);
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
                    // get fish from name
                    // get first destination
                    // mark as sent
                    STAILQ_FIRST(&get_fish_from_name(aquarium, fishes_in_view[iter]->name)->destinations_queue)->is_sent = OK;
                    printf("Marked destination to %dx%d by %ld as sent\n", destination->destination_coordinates.x, destination->destination_coordinates.y, destination->time_at_destination);
                    iter++;

                }
                print_list_fish_for_client(log, fishes_in_view, view, socket_fd, 0); // 0 = first desination



            }

        }

        free_fishes_array(fishes_in_view, view);

        pthread_mutex_unlock(&aquarium_mutex);
        if (minimum_time_to_destination > time(NULL)) {
            sleep(minimum_time_to_destination - time(NULL));
        } else { // in case there is no fish in the view
            sleep(1);
        }
        pthread_mutex_lock(&terminate_threads_mutex);
    }
    pthread_mutex_unlock(&terminate_threads_mutex);
    free(parameters);
    fprintf(log, "===== get_fishes_continuously() terminated =====\n");
    fflush(log);
    fclose(log);
    return NULL;
}