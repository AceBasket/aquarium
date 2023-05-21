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
    struct fish_destination *destination = malloc(sizeof(struct fish_destination));
    destination->destination_coordinates = (struct coordinates){ 23, 3 };
    assert(add_fish(aquarium, fish) == OK);
    assert(add_view(aquarium, view1) == OK);
    assert(add_view(aquarium, view2) == OK);
    assert(add_specific_destination(fish, origin) == OK);
    assert(add_intermediate_movements(aquarium, fish, origin, destination) == OK);
    assert(add_specific_destination(fish, destination) == OK);
    assert(STAILQ_FIRST(&fish->destinations_queue)->destination_coordinates.x == 1);
    assert(STAILQ_FIRST(&fish->destinations_queue)->destination_coordinates.y == 1);
    assert(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next)->destination_coordinates.x == 20);
    assert(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next)->destination_coordinates.y == 2);
    assert(STAILQ_NEXT(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next), next)->destination_coordinates.x == 23);
    assert(STAILQ_NEXT(STAILQ_NEXT(STAILQ_FIRST(&fish->destinations_queue), next), next)->destination_coordinates.y == 3);
    assert(free_aquarium(aquarium) == OK);
}

int main() {
    // printf("Fish tests: .");
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
    test_add_intermediate_movements();
    printf(" OK\n");
    return EXIT_SUCCESS;
}