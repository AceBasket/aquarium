#ifndef _IO_HANDLER_FUNCTIONS_H_
#define _IO_HANDLER_FUNCTIONS_H_

#include "../../aquarium/aquarium.h"
#include "../../parser/parser.h"
// #include "utils.h"

void print_list_fish_for_client(FILE *log, struct fish **fishes_in_view, struct view *view, int socket_fd, int bool_get_next_destination);
void hello_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void get_fishes_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void ping_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void add_fish_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void del_fish_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void start_fish_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void log_out_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
// void status_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void ls_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void get_fishes_continuously_handler(FILE *log, struct parse *parser, int socket_fd, pthread_t *handle_fishes_continuously_thread);

#endif // IO_HANDLER_FUNCTIONS_H_