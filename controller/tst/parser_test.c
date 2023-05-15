#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "../src/utils.h"
#include "../src/parser/client_inputs_parser.h"
#include "../src/parser/prompt_parser.h"



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
    assert(result->size == 7);
    assert(result->func_name == ADDFISH);
    int i = 0;
    assert(!strcmp(result->arguments[i], "PoissonNain"));
    i++;
    // assert(!strcmp(result->arguments[i], "at"));
    // i++;
    assert(!strcmp(result->arguments[i], "61"));
    i++;
    assert(!strcmp(result->arguments[i], "52"));
    i++;
    assert(!strcmp(result->arguments[i], "4"));
    i++;
    assert(!strcmp(result->arguments[i], "3"));
    i++;
    assert(!strcmp(result->arguments[i], "RandomPathway"));

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
    assert(result1->size == 1);
    assert(result1->func_name == DELFISH);
    int i = 0;
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
    assert(result1->size == 1);
    assert(result1->func_name == STARTFISH);
    int i = 0;
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
    assert(result1->size == 3);
    assert(result1->func_name == HELLO);
    int i = 0;
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
    assert(result5->size == 0);
    assert(result5->func_name == HELLO);

    free_parser(result1);
    free_parser(result2);
    free_parser(result3);
    free_parser(result4);
    free_parser(result5);

}

void test_get_fishes() {
    struct parse *result1 = parse_clients("getFishes");
    assert(result1->size == 0);
    assert(result1->func_name == GETFISHES);

    struct parse *result2 = parse_clients("getFishes random");
    // assert(result2 == NULL);
    assert(strcmp(result2->status, "OK") != 0);


    free_parser(result1);
    free_parser(result2);
}

void test_ls() {
    struct parse *result1 = parse_clients("ls");
    assert(result1->size == 0);
    assert(result1->func_name == LS);

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
    assert(result1->size == 0);
    assert(result1->func_name == GFCONTINUOUSLY);

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
    struct parse *result1 = parse_clients("ping 2345");
    assert(result1->size == 1);
    assert(result1->func_name == PING);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "2345"));

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
    assert(result1->size == 1);
    assert(result1->func_name == LOAD);
    int i = 0;
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
    assert(result1->size == 1);
    assert(result1->func_name == SHOW);
    int i = 0;
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
    assert(result1->size == 6);
    assert(result1->func_name == ADD_VIEW);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "view")); // this line should go
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
    assert(result1->size == 2);
    assert(result1->func_name == DEL_VIEW);
    int i = 0;
    assert(!strcmp(result1->arguments[i], "view")); // this line should go
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
    assert(result1->size == 1);
    assert(result1->func_name == SAVE);
    int i = 0;
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
    // test_status();
    // printf("Parser tests: .");
    printf(".");
    test_addFish();
    printf(".");
    test_delFish();
    printf(".");
    test_startFish();
    printf(".");
    test_hello();
    printf(".");
    test_get_fishes();
    printf(".");
    test_ls();
    printf(".");
    test_getFishesContinuously();
    printf(".");
    test_ping();
    // test_log_out(); // not what it should do

    //parse prompt tests
    printf(".");
    test_load();
    printf(".");
    test_show_aquarium();
    printf(".");
    test_add_view();
    printf(".");
    test_del_view();
    printf(".");
    test_save();
    printf(" OK\n");
    return 0;
}