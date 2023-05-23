#include <strings.h>
#include <unistd.h>
#include <sys/types.h> 
#include <pthread.h>
#include <assert.h>
#include <time.h>
// #include "utils.h"
// #include "aquarium/aquarium.h"
#include "communication/controller.h"

#ifndef VERBOSITY_LEVEL
#define VERBOSITY_LEVEL LOG_INFO
#endif

volatile int terminate_threads = NOK;
struct aquarium *aquarium = NULL; // global aquarium
pthread_mutex_t aquarium_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t views_sockets_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t terminate_threads_mutex = PTHREAD_MUTEX_INITIALIZER;
enum log_level verbosity_level = VERBOSITY_LEVEL;

int main(int argc, char const *argv[]) {
    FILE *log = fopen("log_main.log", "w");
    log_message(log, LOG_INFO, "===== thread_main() =====");

    // Checking the number of arguments
    if (argc < 3) {
        log_message(log, LOG_FATAL_ERROR, "Too few arguments (need number of views and port number)");
    }
    // Number of views
    int nb_views = atoi(argv[1]);
    // Port number
    int port_number = atoi(argv[2]);

    if (signal(SIGPIPE, sigpipe_handler) == SIG_ERR) {
        log_message(log, LOG_ERROR, "The signal handler could not be changed");
    }

    // Initialization of the server
    pthread_t tid_accept;
    pthread_t tid_prompt;
    pthread_t tid_io;
    pthread_t tid_timeout;
    struct init_server_parameters parameters = {
        .nb_views = nb_views,
        .port_number = port_number,
        .tid_accept = &tid_accept,
        .tid_prompt = &tid_prompt,
        .tid_io = &tid_io,
        .tid_timeout = &tid_timeout,
        .io_log = fopen("log_io.log", "w"),
        .accept_log = fopen("log_accept.log", "w"),
        .prompt_log = fopen("log_prompt.log", "w"),
        .timeout_log = fopen("log_timeout.log", "w"),
        .prompt_parameters = malloc(sizeof(struct thread_prompt_parameters)),
        .accept_parameters = malloc(sizeof(struct thread_accept_parameters)),
        .io_parameters = malloc(sizeof(struct thread_io_parameters)),
        .views_sockets_fd = malloc(sizeof(int) * MAX_VIEWS)
    };
    // Checking the opening of log files
    if (parameters.io_log == NULL) {
        log_message(log, LOG_ERROR, "The io log file could not be opened");
    } if (parameters.accept_log == NULL) {
        log_message(log, LOG_ERROR, "The accept log file could not be opened");
    } if (parameters.prompt_log == NULL) {
        log_message(log, LOG_ERROR, "The prompt log file could not be opened");
    } if (parameters.timeout_log == NULL) {
        log_message(log, LOG_ERROR, "The timeout log file could not be opened");
    }
    // Checking the memory allocation
    if (parameters.io_parameters == NULL) {
        log_message(log, LOG_ERROR, "The io memory could not be allocated");
    } if (parameters.accept_parameters == NULL) {
        log_message(log, LOG_ERROR, "The accept memory could not be allocated");
    } if (parameters.prompt_parameters == NULL) {
        log_message(log, LOG_ERROR, "The prompt memory could not be allocated");
    }
    init_server(&parameters);

    // Handling fish destinations
    pthread_mutex_lock(&aquarium_mutex);
    pthread_mutex_lock(&terminate_threads_mutex);
    while (aquarium == NULL && terminate_threads == NOK) {
        pthread_mutex_unlock(&terminate_threads_mutex);
        pthread_mutex_unlock(&aquarium_mutex);
        sleep(1);
        pthread_mutex_lock(&aquarium_mutex);
        pthread_mutex_lock(&terminate_threads_mutex);
    }
    pthread_mutex_unlock(&terminate_threads_mutex);
    pthread_mutex_unlock(&aquarium_mutex);

    struct fish *fishes;
    struct fish *current_fish;

    pthread_mutex_lock(&terminate_threads_mutex);
    if (terminate_threads == NOK) {
        pthread_mutex_unlock(&terminate_threads_mutex);
        log_message(log, LOG_INFO, "Aquarium initialized");
        show_aquarium(aquarium, log);
        pthread_mutex_lock(&aquarium_mutex);
        fishes = aquarium->fishes;
        current_fish = fishes;
        pthread_mutex_unlock(&aquarium_mutex);
        pthread_mutex_lock(&terminate_threads_mutex);
    } else {
        pthread_mutex_unlock(&terminate_threads_mutex);
        log_message(log, LOG_INFO, "Aquarium not initialized");
        pthread_mutex_lock(&terminate_threads_mutex);
    }
    pthread_mutex_unlock(&terminate_threads_mutex);

    pthread_mutex_lock(&terminate_threads_mutex);
    while (terminate_threads == NOK) {
        pthread_mutex_unlock(&terminate_threads_mutex);
        pthread_mutex_lock(&aquarium_mutex);
        fishes = aquarium->fishes;
        current_fish = fishes;
        while (current_fish != NULL && current_fish->status == STARTED) {
            remove_finished_movements(aquarium, current_fish);
            if (len_movements_queue(current_fish) < 10) {
                int len = len_movements_queue(current_fish);
                add_movement(aquarium, current_fish);
                log_message(log, LOG_INFO, "Fish %s is at %dx%d and it is %ld", current_fish->name, current_fish->top_left.x, current_fish->top_left.y, get_time_in_milliseconds());
                log_message(log, LOG_INFO, "%s had %d destinations. Movement added", current_fish->name, len);
                debug_destinations_queue(log, current_fish);
            }
            current_fish = current_fish->next;
        }
        pthread_mutex_unlock(&aquarium_mutex);

        if (usleep(200000) != 0) { // 200ms = 0.2 s
            log_message(log, LOG_ERROR, "Usleep() interrupted");
        }
        pthread_mutex_lock(&terminate_threads_mutex);
    }
    pthread_mutex_unlock(&terminate_threads_mutex);

    if (pthread_join(tid_prompt, NULL) != 0) {
        log_message(log, LOG_ERROR, "The prompt thread could not be joined");
    }
    // Checking threads cancellation
    if (pthread_cancel(tid_io) != 0) {
        log_message(log, LOG_ERROR, "The ios thread could not be canceled");
    } // if io thread is waiting in recv --> cancellation point
    if (pthread_cancel(tid_accept) != 0) {
        log_message(log, LOG_ERROR, "The accept thread could not be canceled");
    } // if accept thread is waiting in accept --> cancellation point
    if (pthread_cancel(tid_timeout) != 0) {
        log_message(log, LOG_ERROR, "The timeout thread could not be canceled");
    } // if accept thread is waiting in accept --> cancellation point
    // Checking threads join
    if (pthread_join(tid_io, NULL) != 0) {
        log_message(log, LOG_ERROR, "The io thread could not be joined");
    } if (pthread_join(tid_accept, NULL) != 0) {
        log_message(log, LOG_ERROR, "The accept thread could not be joined");
    } if (pthread_join(tid_timeout, NULL) != 0) {
        log_message(log, LOG_ERROR, "The timeout thread could not be joined");
    }
    // Checking the closing of log files
    if (fclose(parameters.io_log) != 0) {
        log_message(log, LOG_ERROR, "The io stream could not be closed");
    } if (fclose(parameters.accept_log) != 0) {
        log_message(log, LOG_ERROR, "The accept stream could not be closed");
    } if (fclose(parameters.prompt_log) != 0) {
        log_message(log, LOG_ERROR, "The prompt stream could not be closed");
    } if (fclose(parameters.timeout_log) != 0) {
        log_message(log, LOG_ERROR, "The timeout stream could not be closed");
    }

    free(parameters.prompt_parameters);
    free(parameters.accept_parameters);
    free(parameters.io_parameters);
    free(parameters.views_sockets_fd);

    log_message(log, LOG_INFO, "Terminated accept thread");
    log_message(log, LOG_INFO, "===== thread_main() terminated =====");

    fclose(log);

    return EXIT_SUCCESS;
}