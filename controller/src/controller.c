#include <strings.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/select.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include "aquarium.h"
// #include "utils.h"
#include "fish.h"
#include "socket_aquarium.h"
#include "io_handler_functions.h"
#include "prompt_handler_functions.h"
#include "time.h"

// for pthread_yield
#define _GNU_SOURCE
// #include <unistd.h>
// #include <sys/types.h>

#define BUFFER_SIZE 256
#define MAX_VIEWS 8 

struct parameters {
    int nb_views;
    int port;
    // Definition of the current socket and the future socket
    int socket_fd;
    int new_socket_fd;
    // Definition of controller and view address
    struct sockaddr_in ctrl_addr;
    struct sockaddr_in view_addr;
    // Definition of the view address length
    socklen_t view_addr_len;
    int views_sockets[MAX_VIEWS];
    // Set of socket descriptor
    fd_set fds;
};

struct aquarium *aquarium = NULL; // global aquarium
pthread_mutex_t aquarium_mutex = PTHREAD_MUTEX_INITIALIZER;


void *thread_io(void *io) {
    FILE *log = fopen("log_io.log", "w");
    fprintf(log, "===== thread_io() =====\n");
    fflush(log);

    pthread_mutex_lock(&aquarium_mutex);
    while (aquarium == NULL) {
        pthread_mutex_unlock(&aquarium_mutex);
        fprintf(log, "Waiting for aquarium to be initialized\n");
        fflush(log);
        sleep(1);

        pthread_mutex_lock(&aquarium_mutex);
    }
    pthread_mutex_unlock(&aquarium_mutex);

// For communication with views
    fd_set read_fds;
    int *views_socket_fd = (int *)io;
    int recv_bytes;
    char buffer[BUFFER_SIZE];

    while (views_socket_fd[0] == -1) {
        fprintf(log, "Waiting for views to be initialized\n");
        fflush(log);
        sleep(1);
    }

    while (1) {
        FD_ZERO(&read_fds);
        int max_fd = 0;

        for (int num_view = 0; num_view < MAX_VIEWS; num_view++) {
            FD_SET(views_socket_fd[num_view], &read_fds);
            // for select, we need to know the highest file descriptor number
            if (max_fd < views_socket_fd[num_view]) {
                max_fd = views_socket_fd[num_view];
            }
        }

        // Wait indefinitely for an activity on one of the sockets
        fprintf(log, "Waiting for activity on one of the sockets\n");
        fflush(log);
        exit_if(select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1, "ERROR on select");
        for (int num_view = 0; num_view < MAX_VIEWS; num_view++) {
            if (FD_ISSET(views_socket_fd[num_view], &read_fds)) {
                // we have data on the num_view socket
                fprintf(log, "Received data from view %d\n", num_view);
                fflush(log);

                // read data until we get a \n
                int total_recv_bytes = 0; // later on, if we want to keep listening until the client sends a \n
                while (1) {
                    char char_read;
                    recv_bytes = recv(views_socket_fd[num_view], &char_read, 1, 0);
                    exit_if(recv_bytes == -1, "ERROR on recv");
                    if (recv_bytes == 0) {
                        fprintf(log, "Client closed connection\n");
                        fflush(log);
                        break;
                    } else {
                        if (char_read != '\r') {
                            buffer[total_recv_bytes++] = char_read;
                        }
                        if (char_read == '\n') {
                            buffer[total_recv_bytes - 1] = '\0';
                            // we have a full line
                            fprintf(log, "Received %d bytes from view %d: %s\n", total_recv_bytes, num_view, buffer);
                            fflush(log);
                            break;
                        }
                    }
                }


                struct parse *parser = parse_clients(buffer);
                enum func function_called = parser->func_name;
                switch (function_called) {
                case HELLO:
                    fprintf(log, "Hello from view %d\n", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    hello_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case GETFISHES:
                    fprintf(log, "Get fishes from view %d\n", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    get_fishes_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case GFCONTINUOUSLY:
                    fprintf(log, "Get fishes continuously from view %d\n", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    get_fishes_continuously_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case LS:
                    fprintf(log, "List fishes from view %d\n", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    ls_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case PING:
                    fprintf(log, "Ping from view %d\n", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    ping_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case ADDFISH:
                    fprintf(log, "Add fish from view %d\n", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    add_fish_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case DELFISH:
                    fprintf(log, "Delete fish from view %d\n", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    del_fish_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case STARTFISH:
                    fprintf(log, "Start fish (%s) from view %d\n", parser->arguments[0], num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    start_fish_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                case LOG:
                    fprintf(log, "LOGOUT out from view %d\n", num_view);
                    pthread_mutex_lock(&aquarium_mutex);
                    log_out_handler(log, parser, views_socket_fd[num_view], aquarium);
                    pthread_mutex_unlock(&aquarium_mutex);
                    break;
                // case STATUS:
                //     fprintf(log, "Status from view %d\n", num_view);
                //     status_handler(log, parser, views_socket_fd[num_view], aquarium);
                //     break;
                case UNKNOWN:
                    fprintf(log, "Unknown command from view %d\n", num_view);
                    dprintf(views_socket_fd[num_view], "NOK: Unknown command\n");
                    break;
                default:
                    dprintf(views_socket_fd[num_view], "%s", parser->status);
                    break;
                }
                free_parser(parser);
                fflush(log);
            }
        }
    }
    fclose(log);
    return 0;
}


void *thread_prompt() {
    FILE *log = fopen("log_prompt.log", "w");

    fprintf(log, "===== thread_prompt() =====\n");
    fflush(log);

    char buffer[BUFFER_SIZE];
    char char_read;
    int i_buffer;

    while (1) {
        i_buffer = 0;

        // we get the line from the terminal where the user wrote his command
        do {
            char_read = fgetc(stdin);
            buffer[i_buffer] = char_read;
            i_buffer++;
        } while (char_read != '\n' && char_read != EOF);
        buffer[i_buffer - 1] = '\0';

        fprintf(log, "Command: '%s' of size %ld\n", buffer, strlen(buffer));
        fflush(log);

        // we parse this line
        struct parse *parser = parse_prompt(buffer);
        int function = (int)parser->func_name;
        fprintf(log, "Function to execute: %d\n", function);
        fflush(log);

        switch (function) {
        case LOAD:
            fprintf(log, "Loading aquarium from file %s\n", parser->arguments[0]);
            fflush(log);
            pthread_mutex_lock(&aquarium_mutex);
            load_handler(log, parser, &aquarium);
            pthread_mutex_unlock(&aquarium_mutex);
            break;
        case SHOW:
            fprintf(log, "Showing aquarium\n");
            pthread_mutex_lock(&aquarium_mutex);
            show_handler(log, aquarium);
            pthread_mutex_unlock(&aquarium_mutex);
            break;
        case ADD_VIEW:
            fprintf(log, "Adding view %s to the aquarium\n", parser->arguments[1]);
            pthread_mutex_lock(&aquarium_mutex);
            add_view_handler(log, parser, aquarium);
            pthread_mutex_unlock(&aquarium_mutex);
            break;
        case DEL_VIEW:
            fprintf(log, "Deleting view %s from the aquarium\n", parser->arguments[1]);
            pthread_mutex_lock(&aquarium_mutex);
            del_view_handler(log, parser, aquarium);
            pthread_mutex_unlock(&aquarium_mutex);
            break;
        case SAVE:
            fprintf(log, "Saving the aquarium at %s\n", parser->arguments[0]);
            pthread_mutex_lock(&aquarium_mutex);
            save_handler(log, parser, aquarium);
            pthread_mutex_unlock(&aquarium_mutex);
            break;
        default:
            break;
        }
        free_parser(parser);
        fflush(log);
    }

    fclose(log);
    return 0;
}


void *thread_accept(void *param) {
    FILE *log = fopen("log_accept.log", "w");
    struct parameters *p = param;
    int new_socket_fd;
    fprintf(log, "===== thread_accept() =====\n");
    fflush(log);

// Initialization of all views_socket[] to 0 so not checked
    memset(p->views_sockets, -1, sizeof(p->views_sockets));

    pthread_t tid_io;
    exit_if(pthread_create(&tid_io, NULL, thread_io, &p->views_sockets) < 0, "ERROR on thread creation");
        // exit_if(pthread_detach(tid_io) != 0, "ERROR in thread detachment");
    while (1) {

        fprintf(log, "Waiting for a new connection...\n");
        fflush(log);
        p->view_addr_len = sizeof(p->view_addr);
        new_socket_fd = accept(p->socket_fd, (struct sockaddr *)&p->view_addr, &p->view_addr_len);
        exit_if(new_socket_fd < 0, "ERROR on accept");
        fprintf(log, "Welcome\n");
        // Adding the new socket to the array of sockets
        for (int k = 0; k < p->nb_views; k++) {
            // If the position is empty
            if (p->views_sockets[k] == -1) {
                p->views_sockets[k] = new_socket_fd;
                fprintf(log, "Adding to list of sockets as %d\n", k);
                break;
            }
        }
        fflush(log);
    }
    fclose(log);
}


int main(int argc, char const *argv[]) {
    struct parameters param;

    // Checking the number of arguments
    exit_if(argc < 3, "ERROR too few arguments (need number of views and port number)");
    // Number of views
    param.nb_views = atoi(argv[1]);
    // Port number
    param.port = atoi(argv[2]);

    pthread_t tid_accept;
    pthread_t tid_prompt;

    // Creation of the main socket
    param.socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    exit_if(param.socket_fd < 0, "ERROR opening socket");

    // Resetting the controller address area
    bzero((char *)&param.ctrl_addr, sizeof(param.ctrl_addr));
    // Definition of the type of socket created
    param.ctrl_addr.sin_family = AF_INET;
    param.ctrl_addr.sin_addr.s_addr = INADDR_ANY;
    param.ctrl_addr.sin_port = htons(param.port);

    // Bind of the socket to the controller address area 
    exit_if(bind(param.socket_fd, (struct sockaddr *)&param.ctrl_addr, sizeof(param.ctrl_addr)) < 0, "ERROR on binding");

    // Listening to a maximum of nb_views pending connections
    listen(param.socket_fd, param.nb_views);

    exit_if(pthread_create(&tid_accept, NULL, thread_accept, &param) < 0, "ERROR on thread creation");
    exit_if(pthread_create(&tid_prompt, NULL, thread_prompt, NULL) < 0, "ERROR on thread creation");


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
        // fprintf(log, "===== New iteration =====\n");
        // fflush(log);

        pthread_mutex_lock(&aquarium_mutex);
        fishes = aquarium->fishes;
        current_fish = fishes;
        while (current_fish != NULL) {
            remove_finished_movements(current_fish);
            if (len_movements_queue(current_fish) < 5) {
                int len = len_movements_queue(current_fish);
                add_movement(aquarium, current_fish);
                fprintf(log, "%s had %d destinations. Movement added\n", current_fish->name, len);
                // fprintf(log, "Movement added to %s: will go to %dx%d before %ld\n", current_fish->name, (*(current_fish->destinations_queue.stqh_last))->destination_coordinates.x, (*(current_fish->destinations_queue.stqh_last))->destination_coordinates.y, (*(current_fish->destinations_queue.stqh_last))->time_at_destination);
                fprintf(log, "It is actually %ld and %s is at %dx%d\n", time(NULL), current_fish->name, current_fish->top_left.x, current_fish->top_left.y);
                fflush(log);
                assert(len == len_movements_queue(current_fish) - 1);
            }
            current_fish = current_fish->next;
        }
        pthread_mutex_unlock(&aquarium_mutex);

        usleep(200000); // 200ms = 0.2 s
    }

    exit_if(pthread_join(tid_accept, NULL), "ERROR on thread join");
    exit_if(pthread_join(tid_prompt, NULL), "ERROR on thread join");

    // exit_if(close(param.socket_fd) == -1, "ERROR on close");


    return EXIT_SUCCESS;
}