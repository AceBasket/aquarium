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


void *thread_io(void *io) {
    printf("Je suis dans io\n");

    return 0;
}


void *thread_prompt(void *argv) {
    struct parse *parse = parse_prompt(argv);
    int function = (int) parse->func_name;

    switch (function) {
    case LOAD:
        struct parse *file = parse_file(parse->tab[0]);
        struct aquarium *a = create_aquarium(file->tab[0], file->tab[1]);
        struct coordinates coord;
        struct view *v;
        for (int i = 2; i < file->size; i += 5) {
            coord.x = parse->tab[i+1];
            coord.y = parse->tab[i+2];
            v = create_view(file->tab[i], coord, file->tab[i+3], file->tab[i+4]);
            add_view(a, v);
        }
        printf("Aquarium loaded (%d display view)\n", len_views(a));
        break;
    case SHOW:
        show_aquarium(a, stdout);
        break;
    case ADD_VIEW:
        struct coordinates coord = {parse->tab[1], parse->tab[2]};
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
    }

    return 0;
}


void *thread_accept(void *param) {
    struct parameters *p = param;

    // Initialization of all views_socket[] to 0 so not checked
    memset(p->views_sockets, 0, sizeof(p->views_sockets));

    while (1) {
        // Clearing the socket set
        FD_ZERO(&p->fds);
        // Adding the main socket to set
        FD_SET(p->socket_fd, &p->fds);
        int max_fd = p->socket_fd;

        // Adding child sockets to set
        for (int j = 0; j < p->nb_views; j++) {
            // If valid socket descriptor then add to read list
            if (p->views_sockets[j] > 0)
                FD_SET(p->views_sockets[j], &p->fds);

            // If highest file descriptor number, need it for the select function
            if (p->views_sockets[j] > max_fd)
                max_fd = p->views_sockets[j];
        }

        // Wait indefinitely for an activity on one of the sockets
        exit_if(select(max_fd + 1, &p->fds, NULL, NULL, NULL) == -1, "ERROR on select");

        // Something happened on the main socket => incoming connection
        if (FD_ISSET(p->socket_fd, &p->fds)) {
            p->view_addr_len = sizeof(p->view_addr);
            p->new_socket_fd = accept(p->socket_fd, (struct sockaddr *)&p->view_addr, &p->view_addr_len);
            exit_if(p->new_socket_fd < 0, "ERROR on accept");

            printf("Welcome\n");

            // Adding the new socket to the array of sockets
            for (int k = 0; k < p->nb_views; k++) {
                // If the position is empty
                if (p->views_sockets[k] == 0) {
                    p->views_sockets[k] = p->new_socket_fd;
                    printf("Adding to list of sockets as %d\n", k);
                    break;
                }
            }

            pthread_t tid_io;
            exit_if(pthread_create(&tid_io, NULL, thread_io, &p->views_sockets) < 0, "ERROR on thread creation");

            exit_if(pthread_detach(tid_io) != 0, "ERROR in thread detachment");
        }
    }
}


int main(int argc, char const *argv[]) {
    struct parameters param;

    // Checking the number of arguments
    exit_if(argc < 3, "ERROR too few arguments");
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


    return EXIT_SUCCESS;
}