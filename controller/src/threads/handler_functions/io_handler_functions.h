#ifndef _IO_HANDLER_FUNCTIONS_H_
#define _IO_HANDLER_FUNCTIONS_H_

#include <time.h>
#include <unistd.h>
#include "../../parser/parser.h"
#include "list_fishes.h"
#include "../handle_fishes_continuously_thread.h"

/* connects the view or not */
void hello_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
/* gets the list of fish with their destinations */
void get_fishes_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
/* answers pong to the client */
void ping_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
/* adds a fish to the aquarim */
void add_fish_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
/* deletes a fish from the aquarium */
void del_fish_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
/* strats the fish */
void start_fish_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
/* logs out the view */
void log_out_handler(FILE *log, struct parse *parser, int *socket_fd, struct aquarium *aquarium);
/* sends the list of fish with their destinations*/
void ls_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
/* creates a thread that sends destinations for each fish periodicly */
void get_fishes_continuously_handler(FILE *log, struct parse *parser, int socket_fd, pthread_t *handle_fishes_continuously_thread);

#endif // IO_HANDLER_FUNCTIONS_H_