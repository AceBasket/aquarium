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
    struct aquarium *aquarium = ((struct handle_fishes_continuously_parameters *)parameters)->aquarium;
    int socket_fd = ((struct handle_fishes_continuously_parameters *)parameters)->socket_fd;
    pthread_mutex_t *aquarium_mutex = ((struct handle_fishes_continuously_parameters *)parameters)->aquarium_mutex;
    int *prompt_thread_terminated = ((struct handle_fishes_continuously_parameters *)parameters)->prompt_thread_terminated;
    pthread_mutex_t *prompt_thread_terminated_mutex = ((struct handle_fishes_continuously_parameters *)parameters)->prompt_thread_terminated_mutex;
    FILE *log = fopen("log_handle_fishes_continuously.log", "w");
    fprintf(log, "===== get_fishes_continuously() =====\n");
    fflush(log);

    pthread_mutex_lock(aquarium_mutex);
    struct view *view = get_view_from_socket(aquarium, socket_fd);
    pthread_mutex_unlock(aquarium_mutex);
    struct fish **fishes_in_view;
    time_t minimum_time_to_destination = 0;

    pthread_mutex_lock(prompt_thread_terminated_mutex);
    while (*prompt_thread_terminated == NOK) {
        pthread_mutex_unlock(prompt_thread_terminated_mutex);

        pthread_mutex_lock(aquarium_mutex);
        fishes_in_view = get_fishes_in_view_and_with_destination_in_view(aquarium, view, 0); // 0 = false
        if (fishes_in_view[0] == NULL) {
            pthread_mutex_unlock(aquarium_mutex);
            fprintf(log, "Error: no fishes in view\n");
            fflush(log);
            pthread_mutex_lock(aquarium_mutex);
        } else {
            if (STAILQ_EMPTY(&fishes_in_view[0]->destinations_queue)) {
                pthread_mutex_unlock(aquarium_mutex);
                fprintf(log, "Error: no destinations in queue\n");
                fflush(log);
                pthread_mutex_lock(aquarium_mutex);
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

        pthread_mutex_unlock(aquarium_mutex);
        if (minimum_time_to_destination > time(NULL)) {
            sleep(minimum_time_to_destination - time(NULL));
        } else {
            sleep(1);
        }
        pthread_mutex_lock(prompt_thread_terminated_mutex);
    }
    pthread_mutex_unlock(prompt_thread_terminated_mutex);
    fprintf(log, "===== get_fishes_continuously() terminated =====\n");
    fflush(log);
    fclose(log);
    return NULL;
}