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
    fd_set fds;
    int *views_socket_fd = (int *)io;
    int max_fd = 0;
    int recv_bytes;
    char buffer[BUFFER_SIZE];
    FD_ZERO(&fds);

    for (int i = 0; i < MAX_VIEWS; i++) {
        FD_SET(views_socket_fd[i], &fds);
        // for select, we need to know the highest file descriptor number
        if (max_fd < views_socket_fd[i]) {
            max_fd = views_socket_fd[i];
        }
    }

    // Wait indefinitely for an activity on one of the sockets
    exit_if(select(max_fd + 1, &fds, NULL, NULL, NULL) == -1, "ERROR on select");
    for (int i = 0; i < MAX_VIEWS; i++) {
        if (FD_ISSET(views_socket_fd[i], &fds)) {
            recv_bytes = recv(views_socket_fd[i], buffer, BUFFER_SIZE, 0);
            exit_if(recv_bytes == -1, "ERROR on recv");
            // if (recv_bytes == 0), the client has closed the connection (TODO: remove the view from the list)
            printf("Received %d bytes from view %d: %s\n", recv_bytes, i, buffer);
        }
    }



    return 0;
}


void *thread_prompt(void *argv) {
    struct parse *parse = parse_prompt(argv);
    int function = (int)parse->func_name;

    FILE *f;
    struct parse *file;
    struct aquarium *a;
    struct coordinates coord;
    struct view *v;

    switch (function) {
    case LOAD:
        f = fopen(parse->tab[0], "r");
        file = parse_file(f);
        a = create_aquarium(atoi(file->tab[0]), atoi(file->tab[1]));

        for (int i = 2; i < file->size; i += 5) {
            coord.x = atoi(parse->tab[i+1]);
            coord.y = atoi(parse->tab[i+2]);
            v = create_view(file->tab[i], coord, atoi(file->tab[i+3]), atoi(file->tab[i+4]));
            add_view(a, v);
        }
        printf("Aquarium loaded (%d display view)\n", len_views(a));
        break;
    case SHOW:
        if (a == NULL) {
            printf("No aquarium");
            return NULL;
        }
        show_aquarium(a, stdout);
        break;
    case ADD_VIEW:
        coord.x = atoi(parse->tab[1]);
        coord.y = atoi(parse->tab[2]);
        v = create_view(parse->tab[0], coord, atoi(parse->tab[3]), atoi(parse->tab[4]));
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
    int new_socket_fd;
    printf("Je suis dans accept\n");
    // Initialization of all views_socket[] to 0 so not checked
    memset(p->views_sockets, 0, sizeof(p->views_sockets));

    while (1) {

        printf("Waiting for a new connection...\n");
        p->view_addr_len = sizeof(p->view_addr);
        printf("%p %p\n", &p->view_addr, &p->view_addr_len);
        printf("%d\n", p->view_addr_len);
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
    // exit_if(pthread_create(&tid_prompt, NULL, thread_prompt, &argv) < 0, "ERROR on thread creation");

    exit_if(pthread_join(tid_accept, NULL), "ERROR on thread join");
    // exit_if(pthread_join(tid_prompt, NULL), "ERROR on thread join");

    // exit_if(close(param.socket_fd) == -1, "ERROR on close");


    return EXIT_SUCCESS;
}