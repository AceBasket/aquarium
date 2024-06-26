#include "../src/aquarium/aquarium.h"
#include "../src/utils.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void test_create_view() {
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(view->top_left.x == 0);
    assert(view->top_left.y == 0);
    assert(view->width == 50);
    assert(view->height == 50);
    assert(view->next == NULL);
    assert(view->socket_fd == -1);
    assert(strcmp(view->name, "view1") == 0);
    free(view->name);
    free(view);
}

void test_add_view() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    struct view *view2 = create_view("view2", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_view(aquarium, view) == OK);
    assert(add_view(aquarium, view2) == OK);
    assert(aquarium->views == view);
    assert(aquarium->views->next == view2);
    assert(aquarium->views->next->next == NULL);
    assert(free_aquarium(aquarium));
}

void test_add_view_already_in_aquarium() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_view(aquarium, view) == OK);
    assert(add_view(aquarium, view) == NOK);
    assert(free_aquarium(aquarium));
}
void test_add_view_outside_of_aquarium_boundaries() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct view *view1 = create_view("view1", (struct coordinates) { 40, 50 }, 140, 150);
    struct view *view2 = create_view("view2", (struct coordinates) { 90, 90 }, 50, 50);
    assert(add_view(aquarium, view1) == NOK);
    assert(add_view(aquarium, view2) == NOK);
    assert(free_aquarium(aquarium));
}
void test_remove_view() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    struct view *view2 = create_view("view2", (struct coordinates) { 0, 0 }, 50, 50);
    struct view *view3 = create_view("view3", (struct coordinates) { 0, 0 }, 50, 50);
    struct view *view4 = create_view("view4", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_view(aquarium, view) == OK);
    assert(add_view(aquarium, view2) == OK);
    assert(add_view(aquarium, view3) == OK);
    assert(add_view(aquarium, view4) == OK);
    assert(remove_view(aquarium, view4) == OK);
    assert(remove_view(aquarium, view2) == OK);
    assert(remove_view(aquarium, view) == OK);
    assert(remove_view(aquarium, view3) == OK);
    assert(aquarium->views == NULL);
    assert(free_aquarium(aquarium));
}

void test_remove_view_not_in_aquarium() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(remove_view(aquarium, view) == NOK);
    free(view->name);
    free(view);
    assert(free_aquarium(aquarium));
}

void test_get_view() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    struct view *view2 = create_view("view2", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_view(aquarium, view) == OK);
    assert(add_view(aquarium, view2) == OK);
    assert(get_view(aquarium, view->name) == view);
    assert(get_view(aquarium, view2->name) == view2);
    assert(free_aquarium(aquarium));
}

void test_get_views_from_coordinates() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    struct view *view2 = create_view("view2", (struct coordinates) { 25, 0 }, 50, 50);
    struct view *view3 = create_view("view3", (struct coordinates) { 50, 50 }, 50, 50);
    assert(add_view(aquarium, view) == OK);
    assert(add_view(aquarium, view2) == OK);
    assert(add_view(aquarium, view3) == OK);
    struct view **views = get_views_from_coordinates(aquarium, (struct coordinates) { 50, 50 });
    assert(strcmp(views[0]->name, view->name) == 0);
    assert(strcmp(views[1]->name, view2->name) == 0);
    assert(strcmp(views[2]->name, view3->name) == 0);
    assert(views[3] == NULL);
    free_views_array(views);
    assert(free_aquarium(aquarium));
}

int main() {
    printf(".");
    test_create_view();
    printf(".");
    test_add_view();
    printf(".");
    test_add_view_already_in_aquarium();
    printf(".");
    test_add_view_outside_of_aquarium_boundaries();
    printf(".");
    test_remove_view();
    printf(".");
    test_remove_view_not_in_aquarium();
    printf(".");
    test_get_view();
    printf(".");
    test_get_views_from_coordinates();
    printf(" OK\n");

    return EXIT_SUCCESS;
}