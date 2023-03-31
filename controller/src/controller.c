#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "utils.h"

#define BUFFER_SIZE 256




void *thread_controller(void *new_socket_fd) {
    char buffer[BUFFER_SIZE];
    int recv_bytes, send_bytes;
    int socket_fd = *(int *)new_socket_fd;


    while (1) {
        bzero(buffer, BUFFER_SIZE);
        recv_bytes = recv(socket_fd, buffer, BUFFER_SIZE, 0);
        exit_if(recv_bytes < 0, "ERROR reading from socket");

        printf("%d\n", recv_bytes);
        if (recv_bytes == 1) {
            break;
        }

        printf("Here is the message: %s\n", buffer);
        send_bytes = send(socket_fd, buffer, BUFFER_SIZE, 0);
        exit_if(send_bytes < 0, "ERROR writing to socket");
    }

    exit_if(close(socket_fd) == -1, "ERROR on close");

    return EXIT_SUCCESS;
}


int main(int argc, char const *argv[]) {
    int socket_fd, new_socket_fd;
    struct sockaddr_in ctrl_addr, view_addr;
    socklen_t view_addr_len;

    exit_if(argc < 3, "ERROR too few arguments");
    int nb_views = atoi(argv[1]);
    int port = atoi(argv[2]);

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    exit_if(socket_fd < 0, "ERROR opening socket");

    bzero((char *)&ctrl_addr, sizeof(ctrl_addr));
    ctrl_addr.sin_family = AF_INET;
    ctrl_addr.sin_addr.s_addr = INADDR_ANY;
    ctrl_addr.sin_port = htons(port);

    exit_if(bind(socket_fd, (struct sockaddr *)&ctrl_addr, sizeof(ctrl_addr)) < 0, "ERROR on binding");

    // debug
    // char c[16];
    // inet_ntop(AF_INET, &ctrl_addr.sin_addr, c, 16);
    // printf("ctrl addr: %s\n", c);

    listen(socket_fd, 5);

    pthread_t tid[nb_views];

    for (int i = 0; i < nb_views; i++) {
        view_addr_len = sizeof(view_addr);
        new_socket_fd = accept(socket_fd, (struct sockaddr *)&view_addr, &view_addr_len);

        // debug
        // char s[16];
        // inet_ntop(AF_INET, &view_addr.sin_addr, s, 16);
        // printf("view addr: %s\n", s);

        exit_if(new_socket_fd < 0, "ERROR on accept");

        exit_if(pthread_create(&tid[i], NULL, thread_controller, &new_socket_fd) != 0, "ERROR on thread creation");

    }

    for (int i = 0; i < nb_views; i++) {
        exit_if(pthread_join(tid[i], NULL), "ERROR on thread join");
    }

    exit_if(close(socket_fd) == -1, "ERROR on close");
    printf("Finished !\n");


    return EXIT_SUCCESS;
}
