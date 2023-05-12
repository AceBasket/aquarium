#include "../src/aquarium/aquarium.h"
#include "../src/communication/socket_aquarium.h"
#include "../src/utils.h"
#include <assert.h>
#include <stdlib.h>


void test_get_view_from_socket() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    struct view *view2 = create_view("view2", (struct coordinates) { 0, 0 }, 50, 50);
    view->socket_fd = 1;
    view2->socket_fd = 6;
    assert(add_view(aquarium, view) == OK);
    assert(add_view(aquarium, view2) == OK);
    assert(get_view_from_socket(aquarium, 1) == view);
    assert(get_view_from_socket(aquarium, 6) == view2);
    free_aquarium(aquarium);
}

void test_get_view_from_socket_not_in_aquarium() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    view->socket_fd = 1;
    assert(add_view(aquarium, view) == OK);
    assert(get_view_from_socket(aquarium, 2) == NULL);
    free_aquarium(aquarium);
}

void test_get_view_from_socket_no_views() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    assert(get_view_from_socket(aquarium, 1) == NULL);
    free_aquarium(aquarium);
}

int main() {
    printf("Socket aquarium tests: .");
    test_get_view_from_socket();
    printf(".");
    test_get_view_from_socket_not_in_aquarium();
    printf(".");
    test_get_view_from_socket_no_views();
    printf(" OK\n");
    return EXIT_SUCCESS;
}