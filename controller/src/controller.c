#include <strings.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/select.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "utils.h"
#include "aquarium.h"
#include "socket_aquarium.h"
#include "io_handler_functions.h"
#include "time.h"
#include <assert.h>

// for gettid
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

struct aquarium *aquarium; // global aquarium

// en attendant le code de Cassandra
void init_aquarium() {
    aquarium = create_aquarium(100, 100);
    struct view *view1 = create_view("N1", (struct coordinates) { 0, 0 }, 50, 50);
    struct view *view2 = create_view("N2", (struct coordinates) { 50, 0 }, 50, 50);
    struct view *view3 = create_view("N3", (struct coordinates) { 0, 50 }, 50, 50);
    struct view *view4 = create_view("N4", (struct coordinates) { 50, 50 }, 50, 50);
    add_view(aquarium, view1);
    add_view(aquarium, view2);
    add_view(aquarium, view3);
    add_view(aquarium, view4);
}

void *thread_io(void *io) {
    FILE *log = fopen("log_io", "w");
    fprintf(log, "Je suis dans io\n");
    fflush(log);
    // For communication with views
    fd_set read_fds;
    int *views_socket_fd = (int *)io;
    int recv_bytes;
    char buffer[BUFFER_SIZE];

    while (1) {
        FD_ZERO(&read_fds);
        int max_fd = 0;

        for (int i = 0; i < MAX_VIEWS; i++) {
            FD_SET(views_socket_fd[i], &read_fds);
            // for select, we need to know the highest file descriptor number
            if (max_fd < views_socket_fd[i]) {
                max_fd = views_socket_fd[i];
            }
        }

        // Wait indefinitely for an activity on one of the sockets
        exit_if(select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1, "ERROR on select");
        for (int i = 0; i < MAX_VIEWS; i++) {
            if (FD_ISSET(views_socket_fd[i], &read_fds)) {
                fprintf(log, "Data on view %d\n", i);
                fprintf(log, "Socket %d\n", views_socket_fd[i]);
                // we have data on the i-th socket

                // read data until we get a \n
                int total_recv_bytes = 0; // later on, if we want to keep listening until the client sends a \n
                while (1) {
                    char c;
                    recv_bytes = recv(views_socket_fd[i], &c, 1, 0);
                    exit_if(recv_bytes == -1, "ERROR on recv");
                    if (recv_bytes == 0) {
                        fprintf(log, "Client closed connection\n");
                        break;
                    } else {
                        if (c != '\r') {
                            buffer[total_recv_bytes++] = c;
                            // printf("%d ", c);
                        }
                        if (c == '\n') {
                            buffer[total_recv_bytes - 1] = '\0';
                            // we have a full line
                            fprintf(log, "Received %d bytes from view %d: %s\n", total_recv_bytes, i, buffer);
                            break;
                        }
                    }
                    fflush(log);
                }


                struct parse *parser = parse_clients(buffer);
                enum func function_called = parser->func_name;
                switch (function_called) {
                case HELLO:
                    fprintf(log, "Hello from view %d\n", i);
                    hello_handler(parser, views_socket_fd[i], aquarium);
                    break;
                case GETFISHES:
                    fprintf(log, "Get fishes from view %d\n", i);
                    get_fishes_handler(parser, views_socket_fd[i], aquarium);
                    break;
                case GFCONTINUOUSLY:
                    fprintf(log, "Get fishes continuously from view %d\n", i);
                    get_fishes_continuously_handler(parser, views_socket_fd[i], aquarium);
                    break;
                case LS:
                    fprintf(log, "List fishes from view %d\n", i);
                    ls_handler(parser, views_socket_fd[i], aquarium);
                    break;
                case PING:
                    fprintf(log, "Ping from view %d\n", i);
                    ping_handler(parser, views_socket_fd[i], aquarium);
                    break;
                case ADDFISH:
                    fprintf(log, "Add fish from view %d\n", i);
                    add_fish_handler(parser, views_socket_fd[i], aquarium);
                    break;
                case DELFISH:
                    fprintf(log, "Delete fish from view %d\n", i);
                    del_fish_handler(parser, views_socket_fd[i], aquarium);
                    break;
                case STARTFISH:
                    fprintf(log, "Start fish (%s) from view %d\n", parser->tab[1], i);
                    start_fish_handler(parser, views_socket_fd[i], aquarium);
                    break;
                case LOG:
                    fprintf(log, "LOGOUT out from view %d\n", i);
                    log_out_handler(parser, views_socket_fd[i], aquarium);
                    break;
                case STATUS:
                    fprintf(log, "Status from view %d\n", i);
                    status_handler(parser, views_socket_fd[i], aquarium);
                    break;
                case UNKNOWN:
                    fprintf(log, "Unknown command from view %d\n", i);
                    dprintf(views_socket_fd[i], "NOK: Unknown command\n");
                    break;
                default:
                    dprintf(views_socket_fd[i], "%s", parser->status);
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
    FILE *log = fopen("log_prompt", "w");

    fprintf(log, "in thread prompt\n");
    fflush(log);
    char buffer[BUFFER_SIZE];
    char c;
    int i_buffer;

    while (1) {
        i_buffer = 0;

        do {
            c = fgetc(stdin);
            buffer[i_buffer] = c;
            i_buffer++;
        } while (c != '\n' && c != EOF);
        buffer[i_buffer - 1] = '\0';

        fprintf(log, "buffer: %s\n", buffer);
        fflush(log);
        struct parse *parse = parse_prompt(buffer);
        fprintf(log, "function: %s\n", parse->tab[0]);
        fprintf(log, "arg1: %s\n", parse->tab[1]);
        fprintf(log, "func_name: %d\n", parse->func_name);
        fflush(log);
        int function = (int)parse->func_name;
        fprintf(log, "int function : %d\n", function);

        FILE *fd;
        struct parse *file;
        struct coordinates coord;
        struct view *view;

        switch (function) {
        case LOAD:
            fprintf(log, "Loading aquarium from file %s\n", parse->tab[1]);
            fflush(log);
            fd = fopen(parse->tab[1], "r");
            // exit_if(fd == NULL, "ERROR opening file");
            if (fd == NULL) {
                fprintf(stderr, "ERROR opening file\n");
                break;
            }
            fprintf(log, "File opened\n");
            fflush(log);
            file = parse_file(fd);
            fprintf(log, "after parse\n");
            fflush(log);
            fprintf(log, "file->tab[1]: %s\n", file->tab[1]);
            fprintf(log, "file->tab[2]: %s\n", file->tab[2]);
            fflush(log);
            aquarium = create_aquarium(atoi(file->tab[1]), atoi(file->tab[2]));
            fprintf(log, "Aquarium created\n");
            fflush(log);

            for (int i = 2; i < file->size; i += 5) {
                fprintf(log, "i: %d\n", i);
                fprintf(log, "file->tab[i + 1]: %s\n", file->tab[i + 1]);
                fprintf(log, "file->tab[i + 2]: %s\n", file->tab[i + 2]);
                fprintf(log, "file->tab[i + 3]: %s\n", file->tab[i + 3]);
                fprintf(log, "file->tab[i + 4]: %s\n", file->tab[i + 4]);
                fflush(log);

                coord.x = atoi(file->tab[i + 1]);
                coord.y = atoi(file->tab[i + 2]);

                view = create_view(file->tab[i], coord, atoi(file->tab[i + 3]), atoi(file->tab[i + 4]));
                if (view != NULL) {
                    fprintf(log, "view created\n");
                    fflush(log);

                }
                if (add_view(aquarium, view)) {
                    fprintf(log, "view added\n");
                    fflush(log);
                };
            }
            fprintf(log, "Aquarium loaded (%d display view)\n", len_views(aquarium));
            break;
        case SHOW:
            if (aquarium == NULL) {
                fprintf(log, "No aquarium");
                fflush(log);
                return NULL;
            }
            show_aquarium(aquarium, stdout);
            break;
        case ADD_VIEW:
            coord.x = atoi(parse->tab[1]);
            coord.y = atoi(parse->tab[2]);
            view = create_view(parse->tab[0], coord, atoi(parse->tab[3]), atoi(parse->tab[4]));
            add_view(aquarium, view);
            fprintf(log, "View added\n");
            break;
        case DEL_VIEW:
            remove_view(aquarium, get_view(aquarium, parse->tab[0]));
            fprintf(log, "View %s deleted\n", parse->tab[0]);
            break;
        case SAVE:
            save_aquarium(aquarium, parse->tab[0]);
            fprintf(log, "Aquarium saved (%d display view)\n", len_views(aquarium));
            break;
        default:
            break;
        }
    }


    fclose(log);
    return 0;
}


void *thread_accept(void *param) {
    FILE *log = fopen("log_accept", "w");
    struct parameters *p = param;
    int new_socket_fd;
    fprintf(log, "Je suis dans accept\n");
    fflush(log);

// en attendant le code de Cassandra
// fprintf(log, "J'initialise l'aquarium\n");
// init_aquarium();


// Initialization of all views_socket[] to 0 so not checked
    memset(p->views_sockets, -1, sizeof(p->views_sockets));

    pthread_t tid_io;
    exit_if(pthread_create(&tid_io, NULL, thread_io, &p->views_sockets) < 0, "ERROR on thread creation");
        // exit_if(pthread_detach(tid_io) != 0, "ERROR in thread detachment");
    while (1) {

        fprintf(log, "Waiting for a new connection...\n");
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

    exit_if(pthread_join(tid_accept, NULL), "ERROR on thread join");
    exit_if(pthread_join(tid_prompt, NULL), "ERROR on thread join");

    // exit_if(close(param.socket_fd) == -1, "ERROR on close");

    // while (1) {
    //     time_t now = time(NULL);
    //     struct fish *fishes = aquarium->fishes;
    //     struct fish *current_fish = fishes;
    //     while (current_fish != NULL) {
    //         if (current_fish->time_to_destination <= now) {
    //             set_movement(aquarium, current_fish);
    //         }
    //         current_fish = current_fish->next;
    //     }
    //     sleep(1);
    // }


    return EXIT_SUCCESS;
}