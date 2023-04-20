#include "../src/aquarium.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
void test_create_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    assert(a->width == 100);
    assert(a->height == 100);
    assert(a->views == NULL);
    assert(a->fishes == NULL);
}

void test_create_view() {
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(v->top_left.x == 0);
    assert(v->top_left.y == 0);
    assert(v->width == 50);
    assert(v->height == 50);
    assert(v->next == NULL);
    assert(v->socket_fd == -1);
    assert(strcmp(v->name, "view1") == 0);
}

void test_add_view() {
    struct aquarium *a = create_aquarium(100, 100);
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_view(a, v) == EXIT_SUCCESS);
    assert(a->views == v);
    assert(a->views->next == NULL);
}

void test_add_view_already_in_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_view(a, v) == EXIT_SUCCESS);
    assert(add_view(a, v) == EXIT_FAILURE);
}

void test_remove_view() {
    struct aquarium *a = create_aquarium(100, 100);
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_view(a, v) == EXIT_SUCCESS);
    assert(remove_view(a, v) == EXIT_SUCCESS);
    assert(a->views == NULL);
}

void test_remove_view_not_in_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(remove_view(a, v) == EXIT_FAILURE);
}

void test_create_fish() {
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(f->top_left.x == 0);
    assert(f->top_left.y == 0);
    assert(f->destination.x == 0);
    assert(f->destination.y == 0);
    assert(f->height == 10);
    assert(f->width == 10);
    assert(f->time_to_destination == 0);
    assert(f->next == NULL);
    assert(strcmp(f->name, "fish1") == 0);
}

void test_add_fish() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(a, f) == EXIT_SUCCESS);
    assert(a->fishes == f);
    assert(a->fishes->next == NULL);
}

void test_add_fish_already_in_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(a, f) == EXIT_SUCCESS);
    assert(add_fish(a, f) == EXIT_FAILURE);
}

void test_remove_fish() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(a, f) == EXIT_SUCCESS);
    assert(remove_fish(a, f) == EXIT_SUCCESS);
    assert(a->fishes == NULL);
}

void test_remove_fish_not_in_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(remove_fish(a, f) == EXIT_FAILURE);
}

void test_get_fish_from_name() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(a, f) == EXIT_SUCCESS);
    assert(get_fish_from_name(a, "fish1") == f);
}

void test_get_fish_from_name_not_in_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(a, f) == EXIT_SUCCESS);
    assert(get_fish_from_name(a, "fish2") == NULL);
}

void test_get_fishes_in_view() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 1, 1 }, 10, 10, RANDOMWAYPOINT);
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_fish(a, f) == EXIT_SUCCESS);
    assert(get_fishes_in_view(a, v)[0] == NULL); // fish not started
    start_fish(a, f->name);
    assert(add_view(a, v) == EXIT_SUCCESS);
    assert(get_fishes_in_view(a, v)[0] == f);
    assert(get_fishes_in_view(a, v)[1] == NULL);
}

void test_get_fishes_in_view_not_in_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    // struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    // assert(add_fish(a, f) == EXIT_SUCCESS);
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(get_fishes_in_view(a, v)[0] == NULL);
}

void test_start_fish() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(a, f) == EXIT_SUCCESS);
    assert(start_fish(a, f->name) == EXIT_SUCCESS);
    assert(f->time_to_destination == 0);
}

void test_start_fish_not_in_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(start_fish(a, f->name) == EXIT_FAILURE);
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
    test_get_fishes_in_view();
    test_get_fishes_in_view_not_in_aquarium();
    test_start_fish();
    test_start_fish_not_in_aquarium();
    return EXIT_SUCCESS;
}

