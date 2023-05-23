#include "handle_fishes_continuously_thread.h"
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int fd_is_valid(int fd) {
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

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
    log_message(log, LOG_INFO, "View: %s", view->name);
    pthread_mutex_unlock(&aquarium_mutex);
    struct fish **fishes_in_view;

    pthread_mutex_lock(&terminate_threads_mutex);
    while (terminate_threads == NOK && fd_is_valid(socket_fd)) {
        pthread_mutex_unlock(&terminate_threads_mutex);

        pthread_mutex_lock(&aquarium_mutex);
        fishes_in_view = get_fishes_with_destination_in_view(aquarium, view, 0); // 0 = false
        if (fishes_in_view[0] == NULL) {
            pthread_mutex_unlock(&aquarium_mutex);
            log_message(log, LOG_WARNING, "No fish in the view");
            pthread_mutex_lock(&aquarium_mutex);
        } else {
            // checking that there is a destination to send
            if (STAILQ_EMPTY(&fishes_in_view[0]->destinations_queue)) {
                pthread_mutex_unlock(&aquarium_mutex);
                log_message(log, LOG_WARNING, "No destination in the queue");
                pthread_mutex_lock(&aquarium_mutex);
            } else {
                list_fishes_for_client(log, fishes_in_view, view, socket_fd);
            }
        }
        free_fishes_array(fishes_in_view, view);

        pthread_mutex_unlock(&aquarium_mutex);
        sleep(1);
        pthread_mutex_lock(&terminate_threads_mutex);
    }
    pthread_mutex_unlock(&terminate_threads_mutex);
    free(parameters);
    log_message(log, LOG_INFO, "===== get_fishes_continuously() terminated =====");
    fclose(log);
    return NULL;
}