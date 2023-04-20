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
    struct view *v2 = create_view("view2", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_view(a, v) == OK);
    assert(add_view(a, v2) == OK);
    assert(a->views == v);
    assert(a->views->next == v2);
    assert(a->views->next->next == NULL);
}

void test_add_view_already_in_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_view(a, v) == OK);
    assert(add_view(a, v) == NOK);
}

void test_remove_view() {
    struct aquarium *a = create_aquarium(100, 100);
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_view(a, v) == OK);
    assert(remove_view(a, v) == OK);
    assert(a->views == NULL);
}

void test_remove_view_not_in_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(remove_view(a, v) == NOK);
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
    struct fish *f2 = create_fish("fish2", (struct coordinates) { 3, 2 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(a, f) == OK);
    assert(add_fish(a, f2) == OK);
    assert(a->fishes == f);
    assert(a->fishes->next == f2);
    assert(a->fishes->next->next == NULL);
}

void test_add_fish_already_in_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(a, f) == OK);
    assert(add_fish(a, f) == NOK);
}

void test_remove_fish() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(a, f) == OK);
    assert(remove_fish(a, f) == OK);
    assert(a->fishes == NULL);
}

void test_remove_fish_not_in_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(remove_fish(a, f) == NOK);
}

void test_get_fish_from_name() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(a, f) == OK);
    assert(get_fish_from_name(a, "fish1") == f);
    assert(get_fish_from_name(a, "fish1")->name == f->name);
    assert(strcmp(get_fish_from_name(a, "fish1")->name, "fish1") == 0);
}

void test_get_fish_from_name_not_in_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(a, f) == OK);
    assert(get_fish_from_name(a, "fish2") == NULL);
}

void test_start_fish() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    struct fish *f2 = create_fish("fish2", (struct coordinates) { 3, 4 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(a, f) == OK);
    assert(add_fish(a, f2) == OK);
    assert(start_fish(a, f->name) == OK);
    assert(f->status == STARTED);
    assert(start_fish(a, f2->name) == OK);
    assert(f2->status == STARTED);
    assert(f->time_to_destination == 0);
}

void test_start_fish_not_in_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(start_fish(a, f->name) == NOK);
}

void test_get_fishes_in_view() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 1, 1 }, 10, 10, RANDOMWAYPOINT);
    struct fish *f2 = create_fish("fish2", (struct coordinates) { 3, 4 }, 10, 10, RANDOMWAYPOINT);
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_fish(a, f) == OK);
    assert(add_fish(a, f2) == OK);
    assert(get_fishes_in_view(a, v)[0] == NULL); // fish not started
    assert(start_fish(a, f->name) == OK);
    assert(start_fish(a, f2->name) == OK);
    assert(add_view(a, v) == OK);
    assert(get_fishes_in_view(a, v)[0] == f);
    assert(get_fishes_in_view(a, v)[1] == f2);
    assert(get_fishes_in_view(a, v)[2] == NULL);
}

void test_get_fishes_in_view_not_in_aquarium() {
    struct aquarium *a = create_aquarium(100, 100);
    // struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    // assert(add_fish(a, f) == OK);
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(get_fishes_in_view(a, v)[0] == NULL);
}

void test_get_view() {
    struct aquarium *a = create_aquarium(100, 100);
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    struct view *v2 = create_view("view2", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_view(a, v) == OK);
    assert(add_view(a, v2) == OK);
    assert(get_view(a, v->name) == v);
    assert(get_view(a, v2->name) == v2);
}


void test_add_fish_start_fish_get_fishes() {
    struct aquarium *a = create_aquarium(100, 100);
    struct fish *f = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    struct view *v = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(add_view(a, v) == OK);
    v->socket_fd = 6;
    assert(add_fish(a, f) == OK);
    assert(start_fish(a, f->name) == OK);
    assert(get_fishes_in_view(a, get_view(a, v->name))[0] == f);
    assert(get_fishes_in_view(a, v)[1] == NULL);
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
    test_get_view();
    test_add_fish_start_fish_get_fishes();
    return EXIT_SUCCESS;
}

