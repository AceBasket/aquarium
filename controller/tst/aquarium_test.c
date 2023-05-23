#include "../src/aquarium/aquarium.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void test_create_aquarium() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    assert(aquarium->width == 100);
    assert(aquarium->height == 100);
    assert(aquarium->views == NULL);
    assert(aquarium->fishes == NULL);
    assert(free_aquarium(aquarium));
}

int main() {
    printf(".");
    test_create_aquarium();
    printf(" OK\n");

    return EXIT_SUCCESS;
}

