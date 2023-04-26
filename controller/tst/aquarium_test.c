#include "../src/aquarium.h"
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

void test_remove_view() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    struct view *view2 = create_view("view2", (struct coordinates) { 0, 0 }, 50, 50);
    struct view *view3 = create_view("view3", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_view(aquarium, view) == OK);
    assert(add_view(aquarium, view2) == OK);
    assert(add_view(aquarium, view3) == OK);
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

void test_create_fish() {
    struct fish *fish = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(fish->top_left.x == 0);
    assert(fish->top_left.y == 0);
    assert(STAILQ_EMPTY(&fish->destinations_queue));
    assert(fish->height == 10);
    assert(fish->width == 10);
    assert(fish->next == NULL);
    assert(fish->movement_pattern == RANDOMWAYPOINT);
    assert(strcmp(fish->name, "fish1") == 0);
    free(fish->name);
    free(fish);
}

void test_add_fish() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    struct fish *fish2 = create_fish("fish2", (struct coordinates) { 3, 2 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish) == OK);
    assert(add_fish(aquarium, fish2) == OK);
    assert(aquarium->fishes == fish);
    assert(aquarium->fishes->next == fish2);
    assert(aquarium->fishes->next->next == NULL);
    assert(free_aquarium(aquarium));
}

void test_add_fish_already_in_aquarium() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish) == OK);
    assert(add_fish(aquarium, fish) == NOK);
    assert(free_aquarium(aquarium));
}

void test_remove_fish() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    struct fish *fish2 = create_fish("fish2", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish) == OK);
    assert(add_fish(aquarium, fish2) == OK);
    assert(remove_fish(aquarium, fish2) == OK);
    assert(remove_fish(aquarium, fish) == OK);
    assert(aquarium->fishes == NULL);
    assert(free_aquarium(aquarium));
}

void test_remove_fish_not_in_aquarium() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(remove_fish(aquarium, fish) == NOK);
    free(fish->name);
    free(fish);
    assert(free_aquarium(aquarium));
}

void test_get_fish_from_name() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish) == OK);
    assert(get_fish_from_name(aquarium, "fish1") == fish);
    assert(get_fish_from_name(aquarium, "fish1")->name == fish->name);
    assert(strcmp(get_fish_from_name(aquarium, "fish1")->name, "fish1") == 0);
    assert(free_aquarium(aquarium));
}

void test_get_fish_from_name_not_in_aquarium() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    struct fish *fish3 = create_fish("fish3", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish) == OK);
    assert(add_fish(aquarium, fish3) == OK);
    assert(get_fish_from_name(aquarium, "fish2") == NULL);
    assert(free_aquarium(aquarium));
}

void test_start_fish() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    struct fish *fish2 = create_fish("fish2", (struct coordinates) { 3, 4 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish) == OK);
    assert(add_fish(aquarium, fish2) == OK);
    assert(start_fish(aquarium, fish->name) == OK);
    assert(fish->status == STARTED);
    assert(start_fish(aquarium, fish2->name) == OK);
    assert(fish2->status == STARTED);
    assert(free_aquarium(aquarium));
}

void test_start_fish_not_in_aquarium() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(start_fish(aquarium, fish->name) == NOK);
    free(fish->name);
    free(fish);
    assert(free_aquarium(aquarium));
}

void test_get_fishes_in_view() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 1, 1 }, 10, 10, RANDOMWAYPOINT);
    struct fish *fish2 = create_fish("fish2", (struct coordinates) { 3, 4 }, 10, 10, RANDOMWAYPOINT);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_fish(aquarium, fish) == OK);
    assert(add_fish(aquarium, fish2) == OK);
    struct fish **fishes = get_fishes_in_view(aquarium, view, 1);
    assert(fishes[0] == NULL); // fish not started
    assert(start_fish(aquarium, fish->name) == OK);
    assert(start_fish(aquarium, fish2->name) == OK);
    assert(add_view(aquarium, view) == OK);
    free(fishes); // free previous fishes to call get_fishes_in_view again
    fishes = get_fishes_in_view(aquarium, view, 1);
    assert(fishes[0] == fish);
    assert(fishes[1] == fish2);
    assert(fishes[2] == NULL);
    free(fishes);
    assert(free_aquarium(aquarium));
}

void test_get_fishes_in_view_not_in_aquarium() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    struct fish **fishes = get_fishes_in_view(aquarium, view, 1);
    assert(fishes[0] == NULL);
    free(fishes);
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


void test_add_fish_start_fish_get_fishes() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_view(aquarium, view) == OK);
    view->socket_fd = 6;
    assert(add_fish(aquarium, fish) == OK);
    assert(start_fish(aquarium, fish->name) == OK);
    struct fish **fishes = get_fishes_in_view(aquarium, get_view(aquarium, view->name), 1); // true
    assert(fishes[0] == fish);
    assert(fishes[1] == NULL);
    free(fishes);
    assert(free_aquarium(aquarium));
}


int main() {
    test_create_aquarium();
    test_create_view();
    test_add_view();
    test_add_view_already_in_aquarium();
    test_remove_view();
    test_remove_view_not_in_aquarium();
    test_create_fish();
    test_add_fish();
    test_add_fish_already_in_aquarium();
    test_remove_fish();
    test_remove_fish_not_in_aquarium();
    test_get_fish_from_name();
    test_get_fish_from_name_not_in_aquarium();
    test_start_fish();
    test_start_fish_not_in_aquarium();
    test_get_fishes_in_view();
    test_get_fishes_in_view_not_in_aquarium();
    test_get_view();
    test_add_fish_start_fish_get_fishes();
    return EXIT_SUCCESS;
}

