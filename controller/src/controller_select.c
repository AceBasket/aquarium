#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/select.h>
// #include "utils.h"


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



void exit_if(int condition, const char *prefix)
{
    if (condition) {
        if (errno != 0) {
            perror(prefix);
        }
        else {
            fprintf( stderr, "%s\n", prefix );
        }
        exit(1);
    }
}


void *thread_io(void *io) {
    printf("Je suis dans io\n");

    return 0;
}


void *thread_prompt(void *param) {
    struct parameters *p = param;

    // char buffer[BUFFER_SIZE];
    // int recv_bytes, send_bytes;
    // int *views = views_sockets;
    // int socket_fd = views[0];


    // bzero(buffer, BUFFER_SIZE);
    
    // while ((recv_bytes = recv(socket_fd, buffer, BUFFER_SIZE, 0)) > 1) {
    //     buffer[recv_bytes] = '\0';

    //     printf("Here is the message: %s", buffer);
    //     printf("From: %d\n", socket_fd);

    //     send_bytes = send(socket_fd, buffer, BUFFER_SIZE, 0);
    //     exit_if(send_bytes < 0, "ERROR writing to socket");

    //     memset(buffer, 0, BUFFER_SIZE);
    // }

    // exit_if(recv_bytes < 0, "ERROR reading from socket");
    // if (recv_bytes == 1) {
    //     printf("Client disconnected\n");
    //     // fflush(stdout);
    // }

    // exit_if(close(socket_fd) == -1, "ERROR on close");
    // socket_fd = 0;

    //printf("Je suis dans prompt\n");

    // load aquarium
    // printf("Aquarium loaded (%d display view)\n", p->nb_views);

    // show aquarium
    // printf("%dx%d\n", aquarium_width, aquarium_height);
    // for (int i = 0; i < p->nb_views; i++) {
    //     printf("N%d %dx%d+%d+%d\n", i, vue_x, vue_y, vue_width, vue_height);
    // }
    
    // add view N5 400x400+400+200
    // printf("View added\n");

    // del view N5
    //printf("View N%d deleted\n", i);

    // save aquarium2
    // printf("Aquarium saved (%d display view)\n", p->nb_views);


    return 0;
}


void *thread_accept(void* param) {
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
        for (int j = 0; j < p->nb_views ; j++) {
            // If valid socket descriptor then add to read list
            if(p->views_sockets[j] > 0)
                FD_SET(p->views_sockets[j] , &p->fds);
             
            // If highest file descriptor number, need it for the select function
            if(p->views_sockets[j] > max_fd)
                max_fd = p->views_sockets[j];
        }

        // Wait indefinitely for an activity on one of the sockets
        exit_if(select(max_fd + 1 , &p->fds , NULL , NULL , NULL) == -1, "ERROR on select");

        // Something happened on the main socket => incoming connection
        if (FD_ISSET(p->socket_fd, &p->fds)) {
            p->view_addr_len = sizeof(p->view_addr);
            p->new_socket_fd = accept(p->socket_fd, (struct sockaddr *) &p->view_addr, &p->view_addr_len);
            exit_if(p->new_socket_fd < 0, "ERROR on accept");

            printf("Welcome\n");

            // Adding the new socket to the array of sockets
            for (int k = 0; k < p->nb_views; k++) {
                // If the position is empty
                if(p->views_sockets[k] == 0) {
                    p->views_sockets[k] = p->new_socket_fd;
                    printf("Adding to list of sockets as %d\n" , k);
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
    bzero((char *) &param.ctrl_addr, sizeof(param.ctrl_addr));
    // Definition of the type of socket created
    param.ctrl_addr.sin_family = AF_INET;
    param.ctrl_addr.sin_addr.s_addr = INADDR_ANY;
    param.ctrl_addr.sin_port = htons(param.port);

    // Bind of the socket to the controller address area 
    exit_if(bind(param.socket_fd, (struct sockaddr *) &param.ctrl_addr, sizeof(param.ctrl_addr)) < 0, "ERROR on binding");
    
    // Listening to a maximum of nb_views pending connections
    listen(param.socket_fd, param.nb_views);

    exit_if(pthread_create(&tid_accept, NULL, thread_accept, &param) < 0, "ERROR on thread creation");
    exit_if(pthread_create(&tid_prompt, NULL, thread_prompt, &param) < 0, "ERROR on thread creation");



    exit_if(pthread_join(tid_accept, NULL), "ERROR on thread join");
    exit_if(pthread_join(tid_prompt, NULL), "ERROR on thread join");


    // exit_if(close(param.socket_fd) == -1, "ERROR on close");


    return EXIT_SUCCESS;
}
