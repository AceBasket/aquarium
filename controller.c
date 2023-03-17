#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


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


int thread_controller(int port);


int main(int argc, char const *argv[])
{
    int socket_fd, new_socket_fd;
    int port_controller;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in ctrl_addr, view_addr;
    int recv_bytes, send_bytes;


    exit_if(argc < 2, "ERROR no port provided");

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    exit_if(socket_fd < 0, "ERROR opening socket");

    bzero((char *) &ctrl_addr, sizeof(ctrl_addr));

    port_controller = atoi(argv[1]);

    ctrl_addr.sin_family = AF_INET;
    ctrl_addr.sin_addr.s_addr = INADDR_ANY;
    ctrl_addr.sin_port = htons(port_controller);

    exit_if(bind(socket_fd, (struct sockaddr *) &ctrl_addr, sizeof(ctrl_addr)) < 0, "ERROR on binding");
    // debug
    char c[16];
    inet_ntop(AF_INET, &ctrl_addr.sin_addr, c, 16);
    printf("ctrl addr: %s\n", c);
    
    listen(socket_fd, 5);
    new_socket_fd = accept(socket_fd, (struct sockaddr *) &view_addr, (socklen_t *) sizeof(view_addr));
    
    // debug
    char s[16];
    inet_ntop(AF_INET, &view_addr.sin_addr, s, 16);
    printf("view addr: %s\n", s);

    exit_if(new_socket_fd < 0, "ERROR on accept");
    printf("connection accepted\n");

    bzero(buffer, BUFFER_SIZE);
    recv_bytes = recv(new_socket_fd, buffer, BUFFER_SIZE, 0);
    exit_if(recv_bytes < 0, "ERROR reading from socket");

    printf("Here is the message: %s\n",buffer);
    send_bytes = send(new_socket_fd, buffer, BUFFER_SIZE, 0);
    exit_if(send_bytes < 0, "ERROR writing to socket");

    exit_if(close(socket_fd) == -1, "ERROR on close");


    return 0;
}
