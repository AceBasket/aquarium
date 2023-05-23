#include "../src/aquarium/fish.h"
#include "../src/utils.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

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
    assert(free_aquarium(aquarium) == OK);
}

void test_add_fish_already_in_aquarium() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish) == OK);
    assert(add_fish(aquarium, fish) == NOK);
    assert(free_aquarium(aquarium) == OK);
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
    assert(free_aquarium(aquarium) == OK);
}

void test_remove_fish_not_in_aquarium() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(remove_fish(aquarium, fish) == NOK);
    free(fish->name);
    free(fish);
    assert(free_aquarium(aquarium) == OK);
}

void test_get_fish_from_name() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish) == OK);
    assert(get_fish_from_name(aquarium, "fish1") == fish);
    assert(get_fish_from_name(aquarium, "fish1")->name == fish->name);
    assert(strcmp(get_fish_from_name(aquarium, "fish1")->name, "fish1") == 0);
    assert(free_aquarium(aquarium) == OK);
}

void test_get_fish_from_name_not_in_aquarium() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    struct fish *fish3 = create_fish("fish3", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish) == OK);
    assert(add_fish(aquarium, fish3) == OK);
    assert(get_fish_from_name(aquarium, "fish2") == NULL);
    assert(free_aquarium(aquarium) == OK);
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
    assert(free_aquarium(aquarium) == OK);
}

void test_start_fish_not_in_aquarium() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 0, 0 }, 10, 10, RANDOMWAYPOINT);
    assert(start_fish(aquarium, fish->name) == NOK);
    free(fish->name);
    free(fish);
    assert(free_aquarium(aquarium) == OK);
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
    free_fishes_array(fishes, view); // free previous fishes to call get_fishes_in_view again
    fishes = get_fishes_in_view(aquarium, view, 1);
    assert(fishes[0] == fish);
    assert(fishes[1] == fish2);
    assert(fishes[2] == NULL);
    free_fishes_array(fishes, view);
    assert(free_aquarium(aquarium) == OK);
}

void test_get_fishes_in_view_not_in_aquarium() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    struct fish **fishes = get_fishes_in_view(aquarium, view, 1);
    assert(fishes[0] == NULL);
    free_fishes_array(fishes, view);
    free(view->name);
    free(view);
    assert(free_aquarium(aquarium) == OK);
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
    free_fishes_array(fishes, view);
    assert(free_aquarium(aquarium) == OK);
}

void test_get_fishes_with_destination_in_view() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 1, 1 }, 10, 10, RANDOMWAYPOINT);
    struct fish *fish2 = create_fish("fish2", (struct coordinates) { 3, 4 }, 10, 10, RANDOMWAYPOINT);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    struct view_of_destination *view_of_destination = malloc(sizeof(struct view_of_destination));
    view_of_destination->view_name = view->name;
    view_of_destination->is_sent = NOK;
    add_specific_destination(fish, &(struct fish_destination) {.destination_coordinates = { 2, 2 }, .time_at_destination = 0, .views = { view_of_destination, NULL }});
    add_specific_destination(fish2, &(struct fish_destination) {.destination_coordinates = { 2, 2 }, .time_at_destination = 0, .views = { view_of_destination, NULL }});
    add_specific_destination(fish, &(struct fish_destination) {.destination_coordinates = { 51, 3 }, .time_at_destination = 0, .views = { NULL }});
    add_specific_destination(fish2, &(struct fish_destination) {.destination_coordinates = { 51, 3 }, .time_at_destination = 0, .views = { NULL }});
    assert(add_fish(aquarium, fish) == OK);
    assert(add_fish(aquarium, fish2) == OK);
    struct fish **fishes = get_fishes_with_destination_in_view(aquarium, view, 1);
    assert(fishes[0] == NULL); // fish not started
    assert(start_fish(aquarium, fish->name) == OK);
    assert(start_fish(aquarium, fish2->name) == OK);
    assert(add_view(aquarium, view) == OK);
    free_fishes_array(fishes, view); // free previous fishes to call get_fishes_in_view again
    fishes = get_fishes_with_destination_in_view(aquarium, view, 1);
    assert(fishes[0] != NULL);
    assert(fishes[0]->top_left.x == fish->top_left.x);
    assert(fishes[0]->top_left.y == fish->top_left.y);
    assert(fishes[1]->top_left.x == fish2->top_left.x);
    assert(fishes[1]->top_left.y == fish2->top_left.y);
    assert(fishes[2] == NULL);
    free_fishes_array(fishes, view);
    assert(free_aquarium(aquarium) == OK);
}

void test_get_fishes_with_destination_in_view_not_in_aquarium() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 1, 1 }, 10, 10, RANDOMWAYPOINT);
    add_specific_destination(fish, &(struct fish_destination) {.destination_coordinates = { 200, 2 }, .time_at_destination = 0 });
    start_fish(aquarium, fish->name);
    struct fish **fishes = get_fishes_with_destination_in_view(aquarium, view, 1);
    assert(fishes[0] == NULL);
    free_fishes_array(fishes, view);
    free(view->name);
    free(view);
    assert(free_aquarium(aquarium) == OK);
}

void test_get_fishes_with_destination_in_view_no_destination_in_view() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 1, 1 }, 10, 10, RANDOMWAYPOINT);
    struct fish *fish2 = create_fish("fish2", (struct coordinates) { 3, 4 }, 10, 10, RANDOMWAYPOINT);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    add_specific_destination(fish, &(struct fish_destination) {.destination_coordinates = { 51, 3 }, .time_at_destination = 0 });
    add_specific_destination(fish2, &(struct fish_destination) {.destination_coordinates = { 51, 3 }, .time_at_destination = 0 });
    assert(add_fish(aquarium, fish) == OK);
    assert(add_fish(aquarium, fish2) == OK);
    struct fish **fishes = get_fishes_with_destination_in_view(aquarium, view, 1);
    assert(fishes[0] == NULL); // fish not started
    assert(start_fish(aquarium, fish->name) == OK);
    assert(start_fish(aquarium, fish2->name) == OK);
    assert(add_view(aquarium, view) == OK);
    free_fishes_array(fishes, view); // free previous fishes to call get_fishes_in_view again
    fishes = get_fishes_with_destination_in_view(aquarium, view, 1);
    assert(fishes[0] == NULL); // no destination in view
    assert(fishes[1] == NULL);
    free_fishes_array(fishes, view);
    assert(free_aquarium(aquarium) == OK);
}

void test_add_specific_destination() {
    struct fish *fish = create_fish("fish1", (struct coordinates) { 1, 1 }, 10, 10, RANDOMWAYPOINT);
    struct fish_destination *destination = malloc(sizeof(struct fish_destination));
    destination->destination_coordinates = (struct coordinates){ 2, 2 };
    destination->time_at_destination = 0;
    add_specific_destination(fish, destination);
    assert(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next) == NULL);
    assert(STAILQ_FIRST(&fish->destinations_queue)->destination_coordinates.x == 2);
    assert(STAILQ_FIRST(&fish->destinations_queue)->destination_coordinates.y == 2);
    assert(STAILQ_FIRST(&fish->destinations_queue)->time_at_destination == 0);
    free(fish->name);
    free(fish);
    free(destination);
}

void test_coordinates_are_in_view() {
    struct fish *fish = create_fish("fish1", (struct coordinates) { 1, 1 }, 10, 10, RANDOMWAYPOINT);
    struct view *view = create_view("view1", (struct coordinates) { 0, 0 }, 50, 50);
    assert(coordinates_are_in_view(&fish->top_left, view) == 1);
    free(fish->name);
    free(fish);
    free(view->name);
    free(view);
}

void test_len_fishes() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    assert(len_fishes(aquarium) == 0);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 1, 1 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish) == OK);
    assert(len_fishes(aquarium) == 1);
    struct fish *fish2 = create_fish("fish2", (struct coordinates) { 1, 1 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish2) == OK);
    assert(len_fishes(aquarium) == 2);
    assert(free_aquarium(aquarium) == OK);
}

void test_distance() {
    assert(distance((struct coordinates) { 0, 0 }, (struct coordinates) { 0, 0 }) == 0);
    assert(fabs(distance((struct coordinates) { 0, 0 }, (struct coordinates) { 1, 1 }) - sqrt(2)) < 0.0001);
    assert(fabs(distance((struct coordinates) { 0, 0 }, (struct coordinates) { 2, 2 }) - sqrt(8)) < 0.0001);
    assert((int)distance((struct coordinates) { 0, 0 }, (struct coordinates) { 3, 4 }) == 5);
}

void test_add_movement() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 1, 1 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish) == OK);
    assert(add_movement(aquarium, fish) == OK);
    assert(fish->destinations_queue.stqh_first->destination_coordinates.x); // testing existence
    assert(fish->destinations_queue.stqh_first->destination_coordinates.y); // testing existence
    assert(fish->destinations_queue.stqh_first->time_at_destination); // testing existence
    assert(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next) == NULL);
    assert(free_aquarium(aquarium) == OK);
}

void test_update_fish_coordinates() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 1, 1 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish) == OK);
    assert(add_movement(aquarium, fish) == OK);
    assert(update_fish_coordinates(fish) == OK);
    assert(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next) == NULL);
    assert(free_aquarium(aquarium) == OK);
}

void test_remove_finished_movements() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 1, 1 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish) == OK);
    assert(add_movement(aquarium, fish) == OK);
    assert(update_fish_coordinates(fish) == OK);
    assert(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next) == NULL);
    assert(remove_finished_movements(aquarium, fish) == OK);
    assert(free_aquarium(aquarium) == OK);
}

void test_len_movements_queue() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct fish *fish = create_fish("fish1", (struct coordinates) { 1, 1 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish) == OK);
    assert(len_movements_queue(fish) == 0);
    assert(add_movement(aquarium, fish) == OK);
    assert(len_movements_queue(fish) == 1);
    assert(add_movement(aquarium, fish) == OK);
    assert(len_movements_queue(fish) == 2);
    assert(add_movement(aquarium, fish) == OK);
    assert(len_movements_queue(fish) == 3);
    assert(add_movement(aquarium, fish) == OK);
    assert(len_movements_queue(fish) == 4);
    assert(add_movement(aquarium, fish) == OK);
    assert(len_movements_queue(fish) == 5);
    assert(add_movement(aquarium, fish) == OK);
    assert(len_movements_queue(fish) == 6);
    assert(add_movement(aquarium, fish) == OK);
    assert(len_movements_queue(fish) == 7);
    assert(add_movement(aquarium, fish) == OK);
    assert(len_movements_queue(fish) == 8);
    assert(add_movement(aquarium, fish) == OK);
    assert(len_movements_queue(fish) == 9);
    assert(free_aquarium(aquarium) == OK);
}

void test_add_intermediate_movements() {
    struct aquarium *aquarium = create_aquarium(100, 100);
    struct view *view1 = create_view("view1", (struct coordinates) { 0, 0 }, 20, 20);
    struct view *view2 = create_view("view2", (struct coordinates) { 20, 0 }, 20, 20);
    struct fish *fish = create_fish("fish", (struct coordinates) { 1, 1 }, 10, 10, RANDOMWAYPOINT);
    struct fish_destination *origin = malloc(sizeof(struct fish_destination));
    origin->destination_coordinates = (struct coordinates){ 1, 1 };
    origin->time_at_destination = 0;
    struct view_of_destination *view_of_destination = malloc(sizeof(struct view_of_destination));
    view_of_destination->view_name = view1->name;
    view_of_destination->is_sent = NOK;
    origin->views[0] = view_of_destination;
    origin->views[1] = NULL;
    struct fish_destination *destination = malloc(sizeof(struct fish_destination));
    destination->destination_coordinates = (struct coordinates){ 23, 3 };
    assert(add_fish(aquarium, fish) == OK);
    assert(add_view(aquarium, view1) == OK);
    assert(add_view(aquarium, view2) == OK);
    assert(add_specific_destination(fish, origin) == OK);
    assert(STAILQ_FIRST(&fish->destinations_queue)->destination_coordinates.x == 1);
    assert(STAILQ_FIRST(&fish->destinations_queue)->destination_coordinates.y == 1);
    assert(add_intermediate_movements(aquarium, fish, origin, destination, NOK) == OK);
    assert(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next)->destination_coordinates.x == 20);
    assert(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next)->destination_coordinates.y == 3);
    assert(add_specific_destination(fish, destination) == OK);
    assert(STAILQ_NEXT(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next), next)->destination_coordinates.x == 23);
    assert(STAILQ_NEXT(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next), next)->destination_coordinates.y == 3);
    assert(free_aquarium(aquarium) == OK);

    aquarium = create_aquarium(1000, 1000);
    view1 = create_view("view1", (struct coordinates) { 0, 0 }, 500, 500);
    view2 = create_view("view2", (struct coordinates) { 500, 0 }, 500, 500);
    struct view *view3 = create_view("view3", (struct coordinates) { 0, 500 }, 500, 500);
    struct view *view4 = create_view("view4", (struct coordinates) { 500, 500 }, 500, 500);
    fish = create_fish("fish", (struct coordinates) { 42, 229 }, 10, 10, RANDOMWAYPOINT);
    origin = malloc(sizeof(struct fish_destination));
    origin->destination_coordinates = (struct coordinates){ 373, 421 };
    destination = malloc(sizeof(struct fish_destination));
    destination->destination_coordinates = (struct coordinates){ 919,784 };
    assert(add_fish(aquarium, fish) == OK);
    assert(add_view(aquarium, view1) == OK);
    assert(add_view(aquarium, view2) == OK);
    assert(add_view(aquarium, view3) == OK);
    assert(add_view(aquarium, view4) == OK);
    assert(add_specific_destination(fish, origin) == OK);
    assert(add_intermediate_movements(aquarium, fish, origin, destination, NOK) == OK);
    assert(add_specific_destination(fish, destination) == OK);
    assert(STAILQ_FIRST(&fish->destinations_queue)->destination_coordinates.x == 373);
    assert(STAILQ_FIRST(&fish->destinations_queue)->destination_coordinates.y == 421);
    assert(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next)->destination_coordinates.x == 492);
    assert(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next)->destination_coordinates.y == 500);
    assert(STAILQ_NEXT(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next), next)->destination_coordinates.x == 500);
    assert(STAILQ_NEXT(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next), next)->destination_coordinates.y == 505);
    assert(STAILQ_NEXT(STAILQ_NEXT(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next), next), next)->destination_coordinates.x == 919);
    assert(STAILQ_NEXT(STAILQ_NEXT(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next), next), next)->destination_coordinates.y == 784);
    assert(free_aquarium(aquarium) == OK);

    aquarium = create_aquarium(1000, 1000);
    view1 = create_view("view1", (struct coordinates) { 0, 0 }, 500, 500);
    view2 = create_view("view2", (struct coordinates) { 500, 0 }, 500, 500);
    view3 = create_view("view3", (struct coordinates) { 0, 500 }, 500, 500);
    view4 = create_view("view4", (struct coordinates) { 500, 500 }, 500, 500);
    fish = create_fish("fish", (struct coordinates) { 129, 500 }, 10, 10, RANDOMWAYPOINT);
    origin = malloc(sizeof(struct fish_destination));
    origin->destination_coordinates = (struct coordinates){ 135,929 };
    destination = malloc(sizeof(struct fish_destination));
    destination->destination_coordinates = (struct coordinates){ 802,22 };
    assert(add_fish(aquarium, fish) == OK);
    assert(add_view(aquarium, view1) == OK);
    assert(add_view(aquarium, view2) == OK);
    assert(add_view(aquarium, view3) == OK);
    assert(add_view(aquarium, view4) == OK);
    assert(add_specific_destination(fish, origin) == OK);
    assert(add_intermediate_movements(aquarium, fish, origin, destination, NOK) == OK);
    assert(add_specific_destination(fish, destination) == OK);
    assert(STAILQ_FIRST(&fish->destinations_queue)->destination_coordinates.x == 135);
    assert(STAILQ_FIRST(&fish->destinations_queue)->destination_coordinates.y == 929);
    assert(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next)->destination_coordinates.x == 450);
    assert(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next)->destination_coordinates.y == 500);
    assert(STAILQ_NEXT(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next), next)->destination_coordinates.x == 500);
    assert(STAILQ_NEXT(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next), next)->destination_coordinates.y == 433);
    assert(STAILQ_NEXT(STAILQ_NEXT(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next), next), next)->destination_coordinates.x == 802);
    assert(STAILQ_NEXT(STAILQ_NEXT(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next), next), next)->destination_coordinates.y == 22);
    assert(free_aquarium(aquarium) == OK);

    aquarium = create_aquarium(1000, 500);
    view1 = create_view("view1", (struct coordinates) { 0, 0 }, 500, 500);
    view2 = create_view("view2", (struct coordinates) { 500, 0 }, 500, 500);
    view3 = create_view("view3", (struct coordinates) { 250, 0 }, 500, 500);
    fish = create_fish("fish", (struct coordinates) { 500, 393 }, 10, 10, RANDOMWAYPOINT);
    assert(add_fish(aquarium, fish) == OK);
    assert(add_view(aquarium, view1) == OK);
    assert(add_view(aquarium, view2) == OK);
    assert(add_view(aquarium, view3) == OK);
    destination = malloc(sizeof(struct fish_destination));
    destination->destination_coordinates = (struct coordinates){ 96, 470 };
    origin = malloc(sizeof(struct fish_destination));
    origin->destination_coordinates = (struct coordinates){ 500, 393 };
    assert(add_intermediate_movements(aquarium, fish, origin, destination, OK) == OK);
    assert(STAILQ_FIRST(&fish->destinations_queue)->destination_coordinates.x == 250);
    assert(STAILQ_FIRST(&fish->destinations_queue)->destination_coordinates.y == 441);
    assert(add_specific_destination(fish, destination) == OK);
    assert(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next)->destination_coordinates.x == 96);
    assert(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next)->destination_coordinates.y == 470);
    assert(free_aquarium(aquarium) == OK);
}

void test_get_intersections_btw_trajectory_and_views() {
    /* Test the function get_intersections_btw_trajectory_and_views */
    struct view *view1 = create_view("view1", (struct coordinates) { 0, 0 }, 500, 500);
    // struct view *view2 = create_view("view2", (struct coordinates) { 500, 0 }, 500, 500);
    // struct view *view3 = create_view("view3", (struct coordinates) { 0, 500 }, 500, 500);
    struct view *view4 = create_view("view4", (struct coordinates) { 500, 500 }, 500, 500);
    struct view *views[5] = { view1, view4, NULL };
    struct coordinates trajectory[2] = { { 373, 421 }, { 919, 784 } };
    struct coordinates *intersections = get_intersections_btw_trajectory_and_views(views, &trajectory[0], &trajectory[1]);
    assert(intersections[0].x == 492);
    assert(intersections[0].y == 500);
    assert(intersections[1].x == 492);
    assert(intersections[1].y == 500);
    assert(intersections[2].x == 500);
    assert(intersections[2].y == 505);

    struct coordinates trajectory2[2] = { { 135,929 }, { 802,22 } };
    struct coordinates *intersections2 = get_intersections_btw_trajectory_and_views(views, &trajectory2[0], &trajectory2[1]);
    assert(intersections2[0].x == 450);
    assert(intersections2[0].y == 500);
    assert(intersections2[1].x == 450);
    assert(intersections2[1].y == 500);
    assert(intersections2[2].x == 500);
    assert(intersections2[2].y == 433);
    assert(intersections2[3].x == 500);
    assert(intersections2[3].y == 433);

    struct coordinates trajectory3[2] = { { 13, 458 }, { 500, 411 } };
    struct coordinates *intersections3 = get_intersections_btw_trajectory_and_views(views, &trajectory3[0], &trajectory3[1]);
    assert(intersections3[0].x == 250);
    struct coordinates trajectory4[2] = { { 13, 458 }, { 784, 383 } };
    struct coordinates *intersections4 = get_intersections_btw_trajectory_and_views(views, &trajectory4[0], &trajectory4[1]);
    assert(intersections4[0].x == 250);
    // assert(intersections4[0].y == 441);
    assert(intersections4[1].x == 500);

}

int main() {
    printf(".");
    test_create_fish();
    printf(".");
    test_add_fish();
    printf(".");
    test_add_fish_already_in_aquarium();
    printf(".");
    test_remove_fish();
    printf(".");
    test_remove_fish_not_in_aquarium();
    printf(".");
    test_get_fish_from_name();
    printf(".");
    test_get_fish_from_name_not_in_aquarium();
    printf(".");
    test_start_fish();
    printf(".");
    test_start_fish_not_in_aquarium();
    printf(".");
    test_get_fishes_in_view();
    printf(".");
    test_get_fishes_in_view_not_in_aquarium();
    printf(".");
    test_add_fish_start_fish_get_fishes();
    printf(".");
    test_get_fishes_with_destination_in_view();
    printf(".");
    test_get_fishes_with_destination_in_view_not_in_aquarium();
    printf(".");
    test_get_fishes_with_destination_in_view_no_destination_in_view();
    printf(".");
    test_add_specific_destination();
    printf(".");
    test_coordinates_are_in_view();
    printf(".");
    test_len_fishes();
    printf(".");
    test_distance();
    printf(".");
    test_add_movement();
    printf(".");
    test_update_fish_coordinates();
    printf(".");
    test_remove_finished_movements();
    printf(".");
    test_len_movements_queue();
    printf(".");
    test_get_intersections_btw_trajectory_and_views();
    printf(".");
    test_add_intermediate_movements();
    printf(" OK\n");
    return EXIT_SUCCESS;
}