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
#include "time.h"
#include <assert.h>

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

struct aquarium *a; // global aquarium

// en attendant le code de Cassandra
void init_aquarium() {
    a = create_aquarium(100, 100);
    struct view *v1 = create_view("N1", (struct coordinates) { 0, 0 }, 50, 50);
    struct view *v2 = create_view("N2", (struct coordinates) { 50, 0 }, 50, 50);
    struct view *v3 = create_view("N3", (struct coordinates) { 0, 50 }, 50, 50);
    struct view *v4 = create_view("N4", (struct coordinates) { 50, 50 }, 50, 50);
    add_view(a, v1);
    add_view(a, v2);
    add_view(a, v3);
    add_view(a, v4);
}

void *thread_io(void *io) {
    printf("Je suis dans io\n");
    // For communication with views
    fd_set read_fds;
    int *views_socket_fd = (int *)io;
    int recv_bytes;
    char buffer[BUFFER_SIZE];

    // for handling views
    struct view *view;

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
                // we have data on the i-th socket

                // read data until we get a \n
                int total_recv_bytes = 0; // later on, if we want to keep listening until the client sends a \n
                while (1) {
                    char c;
                    recv_bytes = recv(views_socket_fd[i], &c, 1, 0);
                    exit_if(recv_bytes == -1, "ERROR on recv");
                    if (recv_bytes == 0) {
                        printf("Client closed connection\n");
                        break;
                    } else {
                        if (c != '\r') {
                            buffer[total_recv_bytes++] = c;
                            // printf("%d ", c);
                        }
                        if (c == '\n') {
                            buffer[total_recv_bytes - 1] = '\0';
                            // we have a full line
                            printf("Received %d bytes from view %d: %s\n", total_recv_bytes, i, buffer);
                            break;
                        }
                    }
                }


                struct parse *parser = parse_clients(buffer);
                enum func function_called = parser->func_name;
                switch (function_called) {
                case HELLO:
                    if (parser->size == 3) {
                        view = get_view(a, parser->tab[2]);
                        if (view != NULL && view->socket_fd == -1) {
                            view->socket_fd = views_socket_fd[i];
                            dprintf(views_socket_fd[i], "greeting %s\n", view->name);
                        }
                    } else {
                        view = get_first_free_view_socket(a);
                        if (view == NULL) {
                            dprintf(views_socket_fd[i], "no greeting\n");
                        } else {
                            view->socket_fd = views_socket_fd[i];
                            dprintf(views_socket_fd[i], "greeting %s\n", view->name);
                        }
                    }
                    printf("Hello from view %d\n", i);
                    break;
                case GETFISHES:
                    printf("Get fishes from view %d\n", i);
                    view = get_view_from_socket(a, views_socket_fd[i]);
                    struct fish **fishes_in_view = get_fishes_in_view(a, view);
                    dprintf(views_socket_fd[i], "list");
                    int k = 0;
                    while (fishes_in_view[k] != NULL) {
                        dprintf(views_socket_fd[i], " [%s at %dx%d,%dx%d,%ld]", fishes_in_view[k]->name, x_coordinate_to_percentage(view, fishes_in_view[k]->top_left.x), y_coordinate_to_percentage(view, fishes_in_view[k]->top_left.y), x_coordinate_to_percentage(view, fishes_in_view[k]->destination.x), y_coordinate_to_percentage(view, fishes_in_view[k]->destination.y), fishes_in_view[k]->time_to_destination - time(NULL));
                        k++;
                    }
                    dprintf(views_socket_fd[i], "\n");
                    break;
                case GFCONTINUOUSLY:
                    printf("Get fishes continuously from view %d\n", i);
                    break;
                case LS:
                    printf("List fishes from view %d\n", i);
                    break;
                case PING:
                    printf("Ping from view %d\n", i);
                    dprintf(views_socket_fd[i], "pong %s\n", parser->tab[0]);
                    break;
                case ADDFISH:
                    printf("Add fish from view %d\n", i);
                    // à changer une fois que Fatima aura corriger le parseur (stocke trop d'informations inutiles)
                    if (get_fish_from_name(a, parser->tab[1]) != NULL) {
                        dprintf(views_socket_fd[i], "NOK\n");
                        break;
                    }
                    struct fish *f = create_fish(parser->tab[1], (struct coordinates) { atoi(parser->tab[3]), atoi(parser->tab[4]) }, atoi(parser->tab[5]), atoi(parser->tab[6]), RANDOMWAYPOINT);
                    add_fish(a, f);
                    assert(get_fish_from_name(a, parser->tab[1]) != NULL);
                    // add_fish(a, create_fish(parser->tab[1], (struct coordinates) { atoi(parser->tab[3]), atoi(parser->tab[4]) }, atoi(parser->tab[5]), atoi(parser->tab[6]), RANDOMWAYPOINT));
                    printf("addFish %s %s %s %s %s\n", parser->tab[1], parser->tab[3], parser->tab[4], (parser->tab[5]), (parser->tab[6]));
                    dprintf(views_socket_fd[i], "OK\n");
                    break;
                case DELFISH:
                    printf("Delete fish from view %d\n", i);
                    if (remove_fish(a, get_fish_from_name(a, parser->tab[1]))) {
                        dprintf(views_socket_fd[i], "OK\n");
                    } else {
                        dprintf(views_socket_fd[i], "NOK\n");
                    }
                    break;
                case STARTFISH:
                    printf("Start fish from view %d\n", i);
                    if (start_fish(a, parser->tab[1])) {
                        dprintf(views_socket_fd[i], "OK\n");
                    } else {
                        dprintf(views_socket_fd[i], "NOK\n");
                    }
                    break;
                case LOG:
                    printf("LOGOUT out from view %d\n", i);
                    dprintf(views_socket_fd[i], "bye\n");
                    break;
                case STATUS:
                    printf("Status from view %d\n", i);
                    dprintf(views_socket_fd[i], "OK: Connected to controller, %d fishes found", len_fishes(a));
                    struct fish **all_fishes_in_view = get_fishes_in_view(a, get_view_from_socket(a, views_socket_fd[i])); // need to change to be able to get name of view
                    int j = 0;
                    while (fishes_in_view[j] != NULL) {
                        dprintf(views_socket_fd[i], "\tFish %s at %dx%d,%dx%d %s", all_fishes_in_view[j]->name, all_fishes_in_view[j]->top_left.x, all_fishes_in_view[j]->top_left.y, all_fishes_in_view[j]->width, all_fishes_in_view[j]->height, all_fishes_in_view[j]->status == STARTED ? "started" : "notStarted");
                        j++;
                    }
                    break;
                case UNKNOWN:
                default:
                    printf("Unknown command from view %d\n", i);
                    dprintf(views_socket_fd[i], "NOK\n");
                    break;
                }
                free_parser(parser);
            }
        }

    }



    return 0;
}


void *thread_prompt(void *argv) {
    parse_prompt(argv);
    // struct parse *parse = parse_prompt(argv);
    // int function = (int)parse->func_name;

    /* switch (function) {
    case LOAD:
        struct parse *file = parse_file(parse->tab[0]);
        struct aquarium *a = create_aquarium(file->tab[0], file->tab[1]);
        struct coordinates coord;
        struct view *v;
        for (int i = 2; i < file->size; i += 5) {
            coord.x = parse->tab[i + 1];
            coord.y = parse->tab[i + 2];
            v = create_view(file->tab[i], coord, file->tab[i + 3], file->tab[i + 4]);
            add_view(a, v);
        }
        printf("Aquarium loaded (%d display view)\n", len_views(a));
        break;
    case SHOW:
        show_aquarium(a, stdout);
        break;
    case ADD_VIEW:
        struct coordinates coord = { parse->tab[1], parse->tab[2] };
        struct view *v = create_view(parse->tab[0], coord, parse->tab[3], parse->tab[4]);
        add_view(a, v);
        printf("View added\n");
        break;
    case DEL_VIEW:
        remove_view(a, get_view(a, parse->tab[0]));
        printf("View %s deleted\n", parse->tab[0]);
        break;
    case SAVE:
        save_aquarium(a, parse->tab[0]);
        printf("Aquarium saved (%d display view)\n", len_views(a));
        break;
    default:
        break;
    } */

    return 0;
}


void *thread_accept(void *param) {
    struct parameters *p = param;
    int new_socket_fd;
    printf("Je suis dans accept\n");

    // en attendant le code de Cassandra
    printf("J'initialize l'aquarium\n");
    init_aquarium();


    // Initialization of all views_socket[] to 0 so not checked
    memset(p->views_sockets, 0, sizeof(p->views_sockets));

    while (1) {

        printf("Waiting for a new connection...\n");
        p->view_addr_len = sizeof(p->view_addr);
        new_socket_fd = accept(p->socket_fd, (struct sockaddr *)&p->view_addr, &p->view_addr_len);
        exit_if(new_socket_fd < 0, "ERROR on accept");
        printf("Welcome\n");
        // Adding the new socket to the array of sockets
        for (int k = 0; k < p->nb_views; k++) {
            // If the position is empty
            if (p->views_sockets[k] == 0) {
                p->views_sockets[k] = new_socket_fd;
                printf("Adding to list of sockets as %d\n", k);
                break;
            }
        }
        pthread_t tid_io;
        exit_if(pthread_create(&tid_io, NULL, thread_io, &p->views_sockets) < 0, "ERROR on thread creation");

        exit_if(pthread_detach(tid_io) != 0, "ERROR in thread detachment");
    }
}


int main(int argc, char const *argv[]) {
    struct parameters param;

    // Checking the number of arguments
    exit_if(argc < 3, "ERROR too few arguments (need number of views and port number))");
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
    exit_if(pthread_create(&tid_prompt, NULL, thread_prompt, &argv) < 0, "ERROR on thread creation");

    exit_if(pthread_join(tid_accept, NULL), "ERROR on thread join");
    exit_if(pthread_join(tid_prompt, NULL), "ERROR on thread join");

    // exit_if(close(param.socket_fd) == -1, "ERROR on close");

    while (1) {
        time_t now = time(NULL);
        struct fish *fishes = a->fishes;
        struct fish *current_fish = fishes;
        while (current_fish != NULL) {
            if (current_fish->time_to_destination <= now) {
                set_movement(a, current_fish);
            }
            current_fish = current_fish->next;
        }
        sleep(1);
    }


    return EXIT_SUCCESS;
}