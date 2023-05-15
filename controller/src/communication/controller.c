#include <stdlib.h>
#include <strings.h>
#include "../threads/accept_thread.h"
#include "../threads/prompt_thread.h"
#include "../utils.h"

#define BUFFER_SIZE 256
#define MAX_VIEWS 8 






void init_server(int nb_views, int port_number, struct aquarium **aquarium, pthread_mutex_t *aquarium_mutex, pthread_mutex_t *views_sockets_mutex) {
    struct thread_prompt_parameters *prompt_parameters = malloc(sizeof(struct thread_prompt_parameters));
    struct thread_accept_parameters *accept_parameters = malloc(sizeof(struct thread_accept_parameters));
    int *views_sockets_fd = malloc(MAX_VIEWS * sizeof(int));
    for (int i = 0; i < MAX_VIEWS; i++) {
        views_sockets_fd[i] = -1;
    }
    pthread_t tid_accept;
    pthread_t tid_prompt;

    // Creation of the main socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    exit_if(socket_fd < 0, "ERROR opening socket");

    // Resetting the controller address area
    struct sockaddr_in ctrl_addr;
    bzero((char *)&ctrl_addr, sizeof(ctrl_addr));
    // Definition of the type of socket created
    ctrl_addr.sin_family = AF_INET;
    ctrl_addr.sin_addr.s_addr = INADDR_ANY;
    ctrl_addr.sin_port = htons(port_number);

    // Bind of the socket to the controller address area
    exit_if(bind(socket_fd, (struct sockaddr *)&ctrl_addr, sizeof(ctrl_addr)) < 0, "ERROR on binding");

    // Listening to a maximum of nb_views pending connections
    listen(socket_fd, nb_views);

    accept_parameters->nb_views = nb_views;
    accept_parameters->socket_fd = socket_fd;
    accept_parameters->view_addr_len = sizeof(accept_parameters->view_addr);
    accept_parameters->view_addr = ctrl_addr;
    accept_parameters->views_sockets_mutex = views_sockets_mutex;
    accept_parameters->views_sockets = views_sockets_fd;
    accept_parameters->aquarium_mutex = aquarium_mutex;
    accept_parameters->aquarium = aquarium;

    prompt_parameters->aquarium = aquarium;
    prompt_parameters->aquarium_mutex = aquarium_mutex;

    exit_if(pthread_create(&tid_accept, NULL, thread_accept, accept_parameters) < 0, "ERROR on thread creation");
    exit_if(pthread_create(&tid_prompt, NULL, thread_prompt, prompt_parameters) < 0, "ERROR on thread creation");

}