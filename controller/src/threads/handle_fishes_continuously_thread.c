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
            if (STAILQ_EMPTY(&fishes_in_view[0]->destinations_queue)) {
                pthread_mutex_unlock(&aquarium_mutex);
                fprintf(log, "Error: no destinations in queue\n");
                fflush(log);
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
                fprintf(log, "Fish %s:\n", fishes_in_view[0]->name);
                debug_destinations_queue(log, fishes_in_view[0]);
                print_list_fish_for_client(log, fishes_in_view, view, socket_fd, 0); // 0 = false


            }

        }

        free_fishes_array(fishes_in_view, view);

        pthread_mutex_unlock(&aquarium_mutex);
        if (minimum_time_to_destination > time(NULL)) {
            sleep(minimum_time_to_destination - time(NULL));
        } else {
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