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

struct aquarium *aquarium = NULL; // global aquarium
pthread_mutex_t aquarium_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t views_sockets_mutex = PTHREAD_MUTEX_INITIALIZER;


int main(int argc, char const *argv[]) {

    // Checking the number of arguments
    exit_if(argc < 3, "ERROR too few arguments (need number of views and port number)");
    // Number of views
    int nb_views = atoi(argv[1]);
    // Port number
    int port_number = atoi(argv[2]);

    init_server(nb_views, port_number, &aquarium, &aquarium_mutex, &views_sockets_mutex);

    /* Handling fish destinations */
    FILE *log = fopen("log_main.log", "w");
    fprintf(log, "===== thread_main() =====\n");
    fflush(log);

    pthread_mutex_lock(&aquarium_mutex);
    while (aquarium == NULL) {
        pthread_mutex_unlock(&aquarium_mutex);
        sleep(1);
        pthread_mutex_lock(&aquarium_mutex);
    }
    pthread_mutex_unlock(&aquarium_mutex);

    pthread_mutex_lock(&aquarium_mutex);
    struct fish *fishes = aquarium->fishes;
    struct fish *current_fish = fishes;
    pthread_mutex_unlock(&aquarium_mutex);
    while (1) {

        pthread_mutex_lock(&aquarium_mutex);
        fishes = aquarium->fishes;
        current_fish = fishes;
        while (current_fish != NULL) {
            remove_finished_movements(current_fish);
            if (len_movements_queue(current_fish) < 5) {
                int len = len_movements_queue(current_fish);
                add_movement(aquarium, current_fish);
                fprintf(log, "fish %s is at %dx%d and it is %ld\n", current_fish->name, current_fish->top_left.x, current_fish->top_left.y, time(NULL));
                fprintf(log, "%s had %d destinations. Movement added\n", current_fish->name, len);
                fflush(log);
            }
            current_fish = current_fish->next;
        }
        pthread_mutex_unlock(&aquarium_mutex);

        usleep(200000); // 200ms = 0.2 s
    }


    return EXIT_SUCCESS;
}