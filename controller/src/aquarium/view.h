#ifndef _VIEW_H_
#define _VIEW_H_

#include <stdlib.h>
#include "aquarium.h"

struct view {
    char *name;
    int socket_fd;
    struct coordinates top_left;
    int height;
    int width;
    struct view *next;
};

struct view *create_view(char *name, struct coordinates top_left, int height, int width);
int add_view(struct aquarium *aquarium, struct view *view);
int remove_view(struct aquarium *aquarium, struct view *view);
struct view *get_view(struct aquarium *aquarium, char *name);
struct view **get_views_from_coordinates(struct aquarium *aquarium, struct coordinates coordinates);
int len_views(struct aquarium *aquarium);
void free_view(struct view *view);

#endif // _VIEW_H_