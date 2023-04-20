#include "../src/aquarium.h"
#include "../src/socket_aquarium.h"
#include <assert.h>
#include <stdlib.h>


void test_get_view_from_socket() {
    struct aquarium *a = create_aquarium(100, 100);
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    v->socket_fd = 1;
    assert(add_view(a, v) == EXIT_SUCCESS);
    assert(get_view_from_socket(a, 1) == v);
}

void test_get_view_from_socket_not_in_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    v->socket_fd = 1;
    assert(add_view(a, v) == EXIT_SUCCESS);
    assert(get_view_from_socket(a, 2) == NULL);
}

void test_get_view_from_socket_no_views() {
    struct aquarium *a = create_aquarium(100, 100);
    assert(get_view_from_socket(a, 1) == NULL);
}

int main() {
    test_get_view_from_socket();
    test_get_view_from_socket_not_in_aquarium();
    test_get_view_from_socket_no_views();
    return EXIT_SUCCESS;
}