#include <stdlib.h>
#include <strings.h>
#include "../threads/accept_thread.h"
#include "../threads/prompt_thread.h"
#include "../utils.h"
#include "controller.h"
#include "../parser/cfg_file_parser.h"

#define BUFFER_SIZE 256
#define MAX_VIEWS 8 


void init_server(struct init_server_parameters *parameters) {
    // Initialization of the parameters
    int nb_views = parameters->nb_views;
    int port_number = parameters->port_number;
    pthread_t *tid_accept = parameters->tid_accept;
    pthread_t *tid_prompt = parameters->tid_prompt;
    pthread_t *tid_io = parameters->tid_io;
    pthread_t *tid_timeout = parameters->tid_timeout;
    struct thread_prompt_parameters *prompt_parameters = parameters->prompt_parameters;
    struct thread_accept_parameters *accept_parameters = parameters->accept_parameters;
    struct thread_io_parameters *io_parameters = parameters->io_parameters;
    int *views_sockets_fd = parameters->views_sockets_fd;

    FILE *fd = fopen("src/controller.cfg", "r");
    exit_if(fd == NULL, "ERROR on opening file\n");
    struct parse *parsed_file = parse_file(fd);
    fclose(fd);
    int display_timeout_value = atoi(parsed_file->arguments[3]);
    free_parser(parsed_file);

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
    accept_parameters->views_sockets = views_sockets_fd;
    accept_parameters->tid_io = tid_io;
    accept_parameters->tid_timeout = tid_timeout;
    accept_parameters->display_timeout_value = display_timeout_value;
    accept_parameters->io_parameters = io_parameters;
    accept_parameters->io_parameters->log = parameters->io_log;
    accept_parameters->log = parameters->accept_log;

    prompt_parameters->log = parameters->prompt_log;

    exit_if(pthread_create(tid_accept, NULL, thread_accept, accept_parameters) < 0, "ERROR on thread creation");
    exit_if(pthread_create(tid_prompt, NULL, thread_prompt, prompt_parameters) < 0, "ERROR on thread creation");

}