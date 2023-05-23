#ifndef _LIST_FISHES_H_
#define _LIST_FISHES_H_

#include "../../aquarium/fish.h"
#include "../../communication/socket_aquarium.h"

/* prints one destination per fish on the socket, if the first destination was already sent, print the next unsent one */
void list_fishes_for_client(FILE *log, struct fish **fishes_in_view, struct view *view, int socket_fd);

#endif // _LIST_FISHES_H_ 