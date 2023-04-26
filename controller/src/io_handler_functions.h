#ifndef IO_HANDLER_FUNCTIONS_H_
#define IO_HANDLER_FUNCTIONS_H_

#include "aquarium.h"
#include "utils.h"

void hello_handler(struct parse *parser, int socket_fd, struct aquarium *aquarium);
void get_fishes_handler(struct parse *parser, int socket_fd, struct aquarium *aquarium);
void ping_handler(struct parse *parser, int socket_fd, struct aquarium *aquarium);
void add_fish_handler(struct parse *parser, int socket_fd, struct aquarium *aquarium);
void del_fish_handler(struct parse *parser, int socket_fd, struct aquarium *aquarium);
void start_fish_handler(struct parse *parser, int socket_fd, struct aquarium *aquarium);
void log_out_handler(struct parse *parser, int socket_fd, struct aquarium *aquarium);
void status_handler(struct parse *parser, int socket_fd, struct aquarium *aquarium);
void ls_handler(struct parse *parser, int socket_fd, struct aquarium *aquarium);
void get_fishes_continuously_handler(struct parse *parser, int socket_fd, struct aquarium *aquarium);

#endif // IO_HANDLER_FUNCTIONS_H_