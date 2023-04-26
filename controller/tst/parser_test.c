#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "../src/utils.h"


void test_status() {
    struct parse *result1 = parse_clients("status");
    assert(!strcmp(result1->arguments[0], "status"));

    struct parse *result2 = parse_clients("status whatever");
    assert(strcmp(result2->status, "OK") != 0);
    free_parser(result1);
    free_parser(result2);
}

void test_addFish() {
    struct parse *result = parse_clients("addFish PoissonNain at 61x52, 4x3, RandomPathway");
    assert(result->size == 8);
    int i = 0;
    assert(!strcmp(result->arguments[i], "addFish"));
    i++;
    assert(!strcmp(result->arguments[i], "PoissonNain"));
    i++;
    assert(!strcmp(result->arguments[i], "at"));
    i++;
    assert(!strcmp(result->arguments[i], "61"));

    struct parse *result2 = parse_clients("addFish PoissonNain he");
    assert(strcmp(result2->status, "OK") != 0);
    // assert(result2 == NULL);

    struct parse *result3 = parse_clients("addFish PoissonNain at");
    assert(strcmp(result3->status, "OK") != 0);
    // assert(result3 == NULL);

    struct parse *result4 = parse_clients("addFish PoissonNain at hhj");
    assert(strcmp(result4->status, "OK") != 0);
    // assert(result4 == NULL);

    struct parse *result5 = parse_clients("addFish PoissonNain at 65hhj78");
    assert(strcmp(result5->status, "OK") != 0);
    // assert(result5 == NULL);

    struct parse *result6 = parse_clients("addFish PoissonNain at 678x677, 456x789 Rando3m");
    assert(strcmp(result6->status, "OK") != 0);
    // assert(result6 == NULL);

    free_parser(result);
    free_parser(result2);
    free_parser(result3);
    free_parser(result4);
    free_parser(result5);
    free_parser(result6);

}

void test_delFish() {
    struct parse *result1 = parse_clients("delFish fishName");
    assert(result1->size == 2);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "delFish"));
    i++;
    assert(!strcmp(result1->arguments[i], "fishName"));

    struct parse *result2 = parse_clients("delFish");
    assert(strcmp(result2->status, "OK") != 0);
    // assert(result2 == NULL);

    struct parse *result3 = parse_clients("delFish fish'-(è1");
    assert(strcmp(result3->status, "OK") != 0);
    // assert(result3 == NULL);

    struct parse *result4 = parse_clients("delFish fish1 fish2");
    assert(strcmp(result4->status, "OK") != 0);
    // assert(result4 == NULL);

    free_parser(result1);
    free_parser(result2);
    free_parser(result3);
    free_parser(result4);
}

void test_startFish() {
    struct parse *result1 = parse_clients("startFish fishName");
    assert(result1->size == 2);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "startFish"));
    i++;
    assert(!strcmp(result1->arguments[i], "fishName"));


    struct parse *result2 = parse_clients("startFish");
    assert(strcmp(result2->status, "OK") != 0);
    // assert(result2 == NULL);


    struct parse *result3 = parse_clients("startFish fish1 fish2");
    assert(strcmp(result3->status, "OK") != 0);
    // assert(result3 == NULL);

    struct parse *result4 = parse_clients("delFish fish'-(è1");
    assert(strcmp(result4->status, "OK") != 0);
    // assert(result4 == NULL);

    free_parser(result1);
    free_parser(result2);
    free_parser(result3);
    free_parser(result4);
}

void test_hello() {
    struct parse *result1 = parse_clients("hello in as N12");
    assert(result1->size == 4);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "hello"));
    i++;
    assert(!strcmp(result1->arguments[i], "in"));
    i++;
    assert(!strcmp(result1->arguments[i], "as"));
    i++;
    assert(!strcmp(result1->arguments[i], "N12"));

    struct parse *result2 = parse_clients("hello in as 12 HJK");
    assert(strcmp(result2->status, "OK") != 0);
    // assert(result2 == NULL);

    struct parse *result3 = parse_clients("hello 4567");
    assert(strcmp(result3->status, "OK") != 0);
    // assert(result3 == NULL);

    struct parse *result4 = parse_clients("hello in as HJK");
    assert(strcmp(result4->status, "OK") != 0);
    // assert(result4 == NULL);

    struct parse *result5 = parse_clients("hello");
    assert(result5->size == 1);
    i = 0;
    assert(!strcmp(result5->arguments[i], "hello"));

    free_parser(result1);
    free_parser(result2);
    free_parser(result3);
    free_parser(result4);
    free_parser(result5);

}

void test_get_fishes() {
    struct parse *result1 = parse_clients("getFishes");
    assert(result1->size == 1);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "getFishes"));

    struct parse *result2 = parse_clients("getFishes random");
    // assert(result2 == NULL);
    assert(strcmp(result2->status, "OK") != 0);


    free_parser(result1);
    free_parser(result2);
}

void test_ls() {
    struct parse *result1 = parse_clients("ls");
    assert(result1->size == 1);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "ls"));

    struct parse *result2 = parse_clients("lsthis");
    // assert(result2 == NULL);
    assert(strcmp(result2->status, "OK") != 0);

    struct parse *result3 = parse_clients("ls this");
    // assert(result3 == NULL);
    assert(strcmp(result3->status, "OK") != 0);


    free_parser(result1);
    free_parser(result2);
    free_parser(result3);
}

void test_getFishesContinuously() {
    struct parse *result1 = parse_clients("getFishesContinuously");
    assert(result1->size == 1);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "getFishesContinuously"));

    struct parse *result2 = parse_clients("getFishesContinuouslyy");
    // assert(result2 == NULL);
    assert(strcmp(result2->status, "OK") != 0);

    struct parse *result3 = parse_clients("getFishesContinuously this");
    // assert(result3 == NULL);
    assert(strcmp(result3->status, "OK") != 0);


    free_parser(result1);
    free_parser(result2);
    free_parser(result3);
}

void test_ping() {
    struct parse *result1 = parse_clients("ping");
    assert(result1->size == 1);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "ping"));

    struct parse *result2 = parse_clients("ping6");
    // assert(result2 == NULL);
    assert(strcmp(result2->status, "OK") != 0);

    struct parse *result3 = parse_clients("ping plus");
    // assert(result3 == NULL);
    assert(strcmp(result3->status, "OK") != 0);


    free_parser(result1);
    free_parser(result2);
    free_parser(result3);
}

void test_log_out() {
    struct parse *result1 = parse_clients("log out");
    assert(result1->size == 2);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "log"));
    i++;
    assert(!strcmp(result1->arguments[i], "out"));

    struct parse *result2 = parse_clients("log");
    // assert(result2 == NULL);
    assert(strcmp(result2->status, "OK") != 0);

    struct parse *result3 = parse_clients("log uut");
    // assert(result3 == NULL);
    assert(strcmp(result3->status, "OK") != 0);

    struct parse *result4 = parse_clients("log out aquarium");
    // assert(result4 == NULL);
    assert(strcmp(result4->status, "OK") != 0);

    struct parse *result5 = parse_clients("lot out");
    // assert(result5 == NULL);
    assert(strcmp(result5->status, "OK") != 0);


    free_parser(result1);
    free_parser(result2);
    free_parser(result3);
    free_parser(result4);
    free_parser(result5);
}



// Parse_prompt tests 
void test_load() {
    struct parse *result1 = parse_prompt("load aquarium56");
    assert(result1->size == 2);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "load"));
    i++;
    assert(!strcmp(result1->arguments[i], "aquarium56"));

    struct parse *result2 = parse_prompt("load ");
    // assert(result2 == NULL);
    assert(strcmp(result2->status, "OK") != 0);

    struct parse *result3 = parse_prompt("load aquarium598 aquarium");
    // assert(result3 == NULL);
    assert(strcmp(result3->status, "OK") != 0);

    free_parser(result1);
    free_parser(result2);
    free_parser(result3);
}

void test_show_aquarium() {
    struct parse *result1 = parse_prompt("show aquarium56");
    assert(result1->size == 2);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "show"));
    i++;
    assert(!strcmp(result1->arguments[i], "aquarium56"));

    struct parse *result2 = parse_prompt("show ");
    // assert(result2 == NULL);
    assert(strcmp(result2->status, "OK") != 0);

    struct parse *result3 = parse_prompt("show aquarium598 aquarium");
    // assert(result3 == NULL);
    assert(strcmp(result3->status, "OK") != 0);

    free_parser(result1);
    free_parser(result2);
    free_parser(result3);
}

void test_add_view() {
    struct parse *result1 = parse_prompt("add view N5 400x400+400+200");
    assert(result1->size == 7);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "add"));
    i++;
    assert(!strcmp(result1->arguments[i], "view"));
    i++;
    assert(!strcmp(result1->arguments[i], "N5"));
    i++;
    assert(!strcmp(result1->arguments[i], "400"));
    i++;
    assert(!strcmp(result1->arguments[i], "400"));
    i++;
    assert(!strcmp(result1->arguments[i], "400"));
    i++;
    assert(!strcmp(result1->arguments[i], "200"));

    struct parse *result2 = parse_prompt("add");
    // assert(result2 == NULL);
    assert(strcmp(result2->status, "OK") != 0);

    struct parse *result3 = parse_prompt("add view");
    // assert(result3 == NULL);
    assert(strcmp(result3->status, "OK") != 0);

    struct parse *result4 = parse_prompt("add hello");
    // assert(result4 == NULL);
    assert(strcmp(result4->status, "OK") != 0);

    struct parse *result5 = parse_prompt("add view N");
    // assert(result5 == NULL);
    assert(strcmp(result5->status, "OK") != 0);

    struct parse *result6 = parse_prompt("add view N5 400x400400200456a");
    // assert(result6 == NULL);
    assert(strcmp(result6->status, "OK") != 0);



    free_parser(result1);
    free_parser(result2);
    free_parser(result3);
    free_parser(result4);
    free_parser(result5);
    free_parser(result6);
}

void test_del_view() {
    struct parse *result1 = parse_prompt("del view N34");
    assert(result1->size == 3);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "del"));
    i++;
    assert(!strcmp(result1->arguments[i], "view"));
    i++;
    assert(!strcmp(result1->arguments[i], "N34"));

    struct parse *result2 = parse_prompt("del");
    // assert(result2 == NULL);
    assert(strcmp(result2->status, "OK") != 0);

    struct parse *result3 = parse_prompt("del view");
    // assert(result3 == NULL);
    assert(strcmp(result3->status, "OK") != 0);

    struct parse *result4 = parse_prompt("del hello");
    // assert(result4 == NULL);
    assert(strcmp(result4->status, "OK") != 0);

    struct parse *result5 = parse_prompt("del view dfghjk");
    // assert(result5 == NULL);
    assert(strcmp(result5->status, "OK") != 0);

    struct parse *result6 = parse_prompt("del view N5 N7");
    // assert(result6 == NULL);
    assert(strcmp(result6->status, "OK") != 0);

    free_parser(result1);
    free_parser(result2);
    free_parser(result3);
    free_parser(result4);
    free_parser(result5);
    free_parser(result6);
}

void test_save() {
    struct parse *result1 = parse_prompt("save aquarium");
    assert(result1->size == 2);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "save"));
    i++;
    assert(!strcmp(result1->arguments[i], "aquarium"));

    struct parse *result2 = parse_prompt("save");
    // assert(result2 == NULL);
    assert(strcmp(result2->status, "OK") != 0);

    struct parse *result3 = parse_prompt("save aquarium aquarim2");
    // assert(result3 == NULL);
    assert(strcmp(result3->status, "OK") != 0);

    struct parse *result4 = parse_prompt("save (è-_è_)");
    // assert(result4 == NULL);
    assert(strcmp(result4->status, "OK") != 0);

    free_parser(result1);
    free_parser(result2);
    free_parser(result3);
    free_parser(result4);
}
int main(void) {
   //parse clients tests
    test_status();
    test_addFish();
    test_delFish();
    test_startFish();
    test_hello();
    test_get_fishes();
    test_ls();
    test_getFishesContinuously();
    test_ping();
    test_log_out();

    //parse prompt tests
    test_load();
    test_show_aquarium();
    test_add_view();
    test_del_view();
    test_save();
    return 0;
}