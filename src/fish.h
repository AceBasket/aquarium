#ifndef _FISH_H_
#define _FISH_H_

#include "point.h"

struct fish{
    char * name;
    struct point position;
    struct point size;
};

#endif