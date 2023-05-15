#include <strings.h>
#include <unistd.h>
#include <sys/types.h> 
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include "aquarium/aquarium.h"
#include "aquarium/fish.h"
#include "utils.h"

#include "communication/controller.h"



int main(int argc, char const *argv[]) {

    // Checking the number of arguments
    exit_if(argc < 3, "ERROR too few arguments (need number of views and port number)");
    // Number of views
    int nb_views = atoi(argv[1]);
    // Port number
    int port_number = atoi(argv[2]);

    // Initialization of the server
    struct aquarium *aquarium = NULL; // global aquarium
    pthread_mutex_t aquarium_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t views_sockets_mutex = PTHREAD_MUTEX_INITIALIZER;
    int prompt_thread_terminated = NOK;
    pthread_mutex_t prompt_thread_terminated_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_t tid_accept;
    pthread_t tid_prompt;
    pthread_t tid_io;
    struct init_server_parameters parameters = {
        .nb_views = nb_views,
        .port_number = port_number,
        .aquarium = &aquarium,
        .aquarium_mutex = &aquarium_mutex,
        .views_sockets_mutex = &views_sockets_mutex,
        .prompt_thread_terminated = &prompt_thread_terminated,
        .prompt_thread_terminated_mutex = &prompt_thread_terminated_mutex,
        .tid_accept = &tid_accept,
        .tid_prompt = &tid_prompt,
        .tid_io = &tid_io
    };
    init_server(&parameters);

    /* Handling fish destinations */
    FILE *log = fopen("log_main.log", "w");
    fprintf(log, "===== thread_main() =====\n");
    fflush(log);

    pthread_mutex_lock(&aquarium_mutex);
    pthread_mutex_lock(&prompt_thread_terminated_mutex);
    while (aquarium == NULL && prompt_thread_terminated == NOK) {
        pthread_mutex_unlock(&prompt_thread_terminated_mutex);
        pthread_mutex_unlock(&aquarium_mutex);
        sleep(1);
        pthread_mutex_lock(&aquarium_mutex);
        pthread_mutex_lock(&prompt_thread_terminated_mutex);
    }
    pthread_mutex_unlock(&prompt_thread_terminated_mutex);
    pthread_mutex_unlock(&aquarium_mutex);

    pthread_mutex_lock(&aquarium_mutex);
    struct fish *fishes = aquarium->fishes;
    struct fish *current_fish = fishes;
    pthread_mutex_unlock(&aquarium_mutex);
    pthread_mutex_lock(&prompt_thread_terminated_mutex);
    while (prompt_thread_terminated == NOK) {
        pthread_mutex_unlock(&prompt_thread_terminated_mutex);
        pthread_mutex_lock(&aquarium_mutex);
        fishes = aquarium->fishes;
        current_fish = fishes;
        while (current_fish != NULL && current_fish->status == STARTED) {
            remove_finished_movements(current_fish);
            if (len_movements_queue(current_fish) < 5) {
                int len = len_movements_queue(current_fish);
                add_movement(aquarium, current_fish);
                fprintf(log, "fish %s is at %dx%d and it is %ld\n", current_fish->name, current_fish->top_left.x, current_fish->top_left.y, time(NULL));
                fprintf(log, "%s had %d destinations. Movement added\n", current_fish->name, len);
                fflush(log);
                debug_destinations_queue(log, current_fish);
            }
            current_fish = current_fish->next;
        }
        pthread_mutex_unlock(&aquarium_mutex);

        usleep(200000); // 200ms = 0.2 s
        pthread_mutex_lock(&prompt_thread_terminated_mutex);
    }
    pthread_mutex_unlock(&prompt_thread_terminated_mutex);
    pthread_join(tid_prompt, NULL);
    pthread_cancel(tid_accept); // if accept thread is waiting in accept --> cancellation point
    pthread_join(tid_io, NULL);
    pthread_join(tid_accept, NULL);
    fprintf(log, "Terminated accept thread\n");
    fprintf(log, "===== thread_main() terminated =====\n");
    fflush(log);
    fclose(log);
    return EXIT_SUCCESS;
}