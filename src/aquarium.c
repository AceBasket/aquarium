#include "fish.h"
#include "view.h"


struct aquarium{
    struct point Aquarium_dimensions;
    struct fish fishes[];
    struct view views[];
}

