#ifndef _IO_HANDLER_FUNCTIONS_H_
#define _IO_HANDLER_FUNCTIONS_H_

<<<<<<< HEAD:controller/src/io_handler_functions.h
#include <time.h>
#include <unistd.h>
#include "aquarium.h"
=======
#include "../../aquarium/aquarium.h"
#include "../../parser/parser.h"
>>>>>>> 6d94c0a14232168e20757cfea91708f8a2cd1104:controller/src/threads/handler_functions/io_handler_functions.h
// #include "utils.h"

void hello_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void get_fishes_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void ping_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void add_fish_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void del_fish_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void start_fish_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void log_out_handler(FILE *log, struct parse *parser, int *socket_fd, struct aquarium *aquarium);
void status_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void ls_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);
void get_fishes_continuously_handler(FILE *log, struct parse *parser, int socket_fd, struct aquarium *aquarium);

#endif // IO_HANDLER_FUNCTIONS_H_