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


#define BUFFER_SIZE 256


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


void *thread_controller(void *new_socket_fd) {
    char buffer[BUFFER_SIZE];
    int recv_bytes, send_bytes;
    int socket_fd = *(int *) new_socket_fd;


    bzero(buffer, BUFFER_SIZE);
    
    while ((recv_bytes = recv(socket_fd, buffer, BUFFER_SIZE, 0)) > 1) {
        buffer[recv_bytes] = '\0';

        printf("Here is the message: %s", buffer);
        printf("From: %d\n", socket_fd);

        send_bytes = send(socket_fd, buffer, BUFFER_SIZE, 0);
        exit_if(send_bytes < 0, "ERROR writing to socket");

        memset(buffer, 0, BUFFER_SIZE);
    }

    exit_if(recv_bytes < 0, "ERROR reading from socket");
    if (recv_bytes == 1) {
        printf("Client disconnected\n");
        // fflush(stdout);
    }

    exit_if(close(socket_fd) == -1, "ERROR on close");
    socket_fd = 0;

    return 0;
}


int main(int argc, char const *argv[]) {
    // Definition of the current socket and the future socket
    int socket_fd, new_socket_fd;
    // Definition of controller and view address
    struct sockaddr_in ctrl_addr, view_addr;
    // Definition of the view address length
    socklen_t view_addr_len;

    // Checking the number of arguments
    exit_if(argc < 3, "ERROR too few arguments");
    // Number of views
    int nb_views = atoi(argv[1]);
    // Port number
    int port = atoi(argv[2]);

    // 
    int views_sockets[nb_views];
    // Set of socket descriptor
    fd_set fds;

    // Initialization of all views_socket[] to 0 so not checked
    memset(views_sockets, 0, sizeof(views_sockets));

    // Creation of the main socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    exit_if(socket_fd < 0, "ERROR opening socket");

    // Resetting the controller address area
    bzero((char *) &ctrl_addr, sizeof(ctrl_addr));
    // Definition of the type of socket created
    ctrl_addr.sin_family = AF_INET;
    ctrl_addr.sin_addr.s_addr = INADDR_ANY;
    ctrl_addr.sin_port = htons(port);

    // Bind of the socket to the controller address area 
    exit_if(bind(socket_fd, (struct sockaddr *) &ctrl_addr, sizeof(ctrl_addr)) < 0, "ERROR on binding");
    
    // Listening to a maximum of nb_views pending connections
    listen(socket_fd, nb_views);

    // Creation of nb_views thread (one for each views)

    while (1) {
        // Clearing the socket set
        FD_ZERO(&fds);
        // Adding the main socket to set
        FD_SET(socket_fd, &fds);
        int max_fd = socket_fd;

        // Adding child sockets to set
        for (int j = 0; j < nb_views ; j++) {
            // Socket descriptor
            int sd = views_sockets[j];
             
            // If valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET(sd , &fds);
             
            // If highest file descriptor number, need it for the select function
            if(sd > max_fd)
                max_fd = sd;
        }

        // Wait indefinitely for an activity on one of the sockets
        int activity = select(max_fd + 1 , &fds , NULL , NULL , NULL);
        exit_if(activity == -1, "ERROR on select");

        // Something happened on the main socket => incoming connection
        if (FD_ISSET(socket_fd, &fds)) {
            view_addr_len = sizeof(view_addr);
            new_socket_fd = accept(socket_fd, (struct sockaddr *) &view_addr, &view_addr_len);
            exit_if(new_socket_fd < 0, "ERROR on accept");

            printf("Welcome\n");

            // Adding the new socket to the array of sockets
            for (int k = 0; k < nb_views; k++) {
                // If the position is empty
                if(views_sockets[k] == 0) {
                    views_sockets[k] = new_socket_fd;
                    printf("Adding to list of sockets as %d\n" , k);
                    
                    pthread_t tid;
                    exit_if(pthread_create(&tid, NULL, thread_controller, &views_sockets[k]) < 0, "ERROR on thread creation");
                    
                    exit_if(pthread_detach(tid) != 0, "ERROR in thread detachment");
                    
                    break;
                }
            }      
        }
    }

    // for (int i = 0; i < nb_views; i++) {
    //     exit_if(pthread_join(tid[i], NULL), "ERROR on thread join");
    // }

    // exit_if(close(socket_fd) == -1, "ERROR on close");


    return EXIT_SUCCESS;
}
