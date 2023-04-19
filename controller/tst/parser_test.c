#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "../src/utils.h"


void test_status(){
    struct parse *result1 = parse_clients("status");
    assert(!strcmp(result1->tab[0], "status"));
    free_parser(result1);

    struct parse *result2 = parse_clients("status whatever");
    assert(result2==NULL);
}
 
 void test_addFish(){
    struct parse *result = parse_clients("addFish PoissonNain at 61x52, 4x3, RandomPathway");
     assert(result->size == 8);
     int i=0;
    assert(!strcmp(result->tab[i], "addFish"));
    i++;
    assert(!strcmp(result->tab[i], "PoissonNain"));
    i++;
    assert(!strcmp(result->tab[i], "at"));
    i++;
    assert(!strcmp(result->tab[i], "61"));

    free_parser(result);
    struct parse *result2 = parse_clients("addFish PoissonNain he");
    assert(result2==NULL);
    
    struct parse *result3 = parse_clients("addFish PoissonNain at");
    assert(result3==NULL);

    struct parse *result4 = parse_clients("addFish PoissonNain at hhj");
    assert(result4==NULL);

    struct parse *result5 = parse_clients("addFish PoissonNain at 65hhj78");
    assert(result5==NULL);

    struct parse *result6 = parse_clients("addFish PoissonNain at 678x677, 456x789 Rando3m");
    assert(result6==NULL);
    
 }
 
 void test_delFish(){
    struct parse *result1 = parse_clients("delFish fishName");
    assert(result1->size == 2);
     int i=0;
    assert(!strcmp(result1->tab[i], "delFish"));
    i++;
    assert(!strcmp(result1->tab[i], "fishName"));
    free_parser(result1);
 
    struct parse *result2 = parse_clients("delFish");
    assert(result2==NULL);
     
    struct parse *result3 = parse_clients("delFish fish'-(è1");
    assert(result3==NULL);

    struct parse *result4 = parse_clients("delFish fish1 fish2");
    assert(result4==NULL);
 }
 
 void test_startFish(){
    struct parse *result1 = parse_clients("startFish fishName");
    assert(result1->size == 2);
     int i=0;
    assert(!strcmp(result1->tab[i], "startFish"));
    i++;
    assert(!strcmp(result1->tab[i], "fishName"));
    free_parser(result1);
     

    struct parse *result2 = parse_clients("startFish");
    assert(result2==NULL);
     

    struct parse *result3 = parse_clients("startFish fish1 fish2");
    assert(result3==NULL);

    struct parse *result4 = parse_clients("delFish fish'-(è1");
    assert(result4==NULL);
 }
 
 void test_hello(){
    struct parse *result1 = parse_clients("hello in as 12");
    assert(result1->size == 4);
     int i=0;
    assert(!strcmp(result1->tab[i], "hello"));
    i++;
    assert(!strcmp(result1->tab[i], "in"));
    i++;
    assert(!strcmp(result1->tab[i], "as"));
    i++;
    assert(!strcmp(result1->tab[i], "12"));
    free_parser(result1);

    struct parse *result2 = parse_clients("hello in as 12 HJK");
    assert(result2==NULL);

    struct parse *result3= parse_clients("hello 4567");
    assert(result3==NULL);
 
    struct parse *result4 = parse_clients("hello in as HJK");
    assert(result4==NULL);

    struct parse *result5 = parse_clients("hello");
    assert(result5->size == 1);
    i=0;
    assert(!strcmp(result5->tab[i], "hello"));
    free_parser(result5);
    
}

void test_get_fishes(){
    struct parse *result1 = parse_clients("getFishes");
    assert(result1->size == 1);
    int i=0;
    assert(!strcmp(result1->tab[i], "getFishes"));
    free_parser(result1);

    struct parse *result2 = parse_clients("getFishes random");
    assert(result2 == NULL);
}

void test_ls(){
    struct parse *result1 = parse_clients("ls");
    assert(result1->size == 1);
    int i=0;
    assert(!strcmp(result1->tab[i], "ls"));
    free_parser(result1);
 
    struct parse *result2 = parse_clients("lsthis");
    assert(result2 == NULL);
 
    struct parse *result3 = parse_clients("ls this");
    assert(result3 == NULL);
 }
 
void test_getFishesContinuously(){
    struct parse *result1 = parse_clients("getFishesContinuously");
    assert(result1->size == 1);
    int i=0;
    assert(!strcmp(result1->tab[i], "getFishesContinuously"));
    free_parser(result1);

    struct parse *result2 = parse_clients("getFishesContinuouslyy");
    assert(result2 == NULL);

    struct parse *result3 = parse_clients("getFishesContinuously this");
    assert(result3 == NULL);
}

void test_ping(){
    struct parse *result1 = parse_clients("ping");
    assert(result1->size == 1);
    int i=0;
    assert(!strcmp(result1->tab[i], "ping"));
    free_parser(result1);

    struct parse *result2 = parse_clients("ping6");
    assert(result2 == NULL);

    struct parse *result3 = parse_clients("ping plus");
    assert(result3 == NULL);
}

void test_log_out(){
    struct parse *result1 = parse_clients("log out");
    assert(result1->size == 2);
    int i=0;
    assert(!strcmp(result1->tab[i], "log"));
    i++;
    assert(!strcmp(result1->tab[i], "out"));
    free_parser(result1);

    struct parse *result2 = parse_clients("log");
    assert(result2 == NULL);

    struct parse *result3 = parse_clients("log uut");
    assert(result3 == NULL);

    struct parse *result4 = parse_clients("log out aquarium");
    assert(result4 == NULL);

    struct parse *result5 = parse_clients("lot out");
    assert(result5 == NULL);
}



// Parse_prompt tests 
void test_load(){
    struct parse *result1 = parse_prompt("load aquarium56");
    assert(result1->size == 2);
    int i=0;
    assert(!strcmp(result1->tab[i], "load"));
    i++;
    assert(!strcmp(result1->tab[i], "aquarium56"));
    free_parser(result1);

    struct parse *result2 = parse_prompt("load ");
    assert(result2 == NULL);

    struct parse *result3 = parse_prompt("load aquarium598 aquarium");
    assert(result3 == NULL);

}

void test_show_aquarium(){
    struct parse *result1 = parse_prompt("show aquarium56");
    assert(result1->size == 2);
    int i=0;
    assert(!strcmp(result1->tab[i], "show"));
    i++;
    assert(!strcmp(result1->tab[i], "aquarium56"));
    free_parser(result1);

    struct parse *result2 = parse_prompt("show ");
    assert(result2 == NULL);

    struct parse *result3 = parse_prompt("show aquarium598 aquarium");
    assert(result3 == NULL);
}

void test_add_view(){
    struct parse *result1 = parse_prompt("add view N5 400x400+400+200");
    assert(result1->size == 7);
    int i=0;
    assert(!strcmp(result1->tab[i], "add"));
    i++;
    assert(!strcmp(result1->tab[i], "view"));
    i++;
    assert(!strcmp(result1->tab[i], "N5"));
    i++;
    assert(!strcmp(result1->tab[i], "400"));
    i++;
    assert(!strcmp(result1->tab[i], "400"));
    i++;
    assert(!strcmp(result1->tab[i], "400"));
    i++;
    assert(!strcmp(result1->tab[i], "200"));
    free_parser(result1);

    struct parse *result2 = parse_prompt("add");
    assert(result2==NULL);
    
    struct parse *result3 = parse_prompt("add view");
    assert(result3==NULL);

    struct parse *result4 = parse_prompt("add hello");
    assert(result4==NULL);

    struct parse *result5 = parse_prompt("add view N");
    assert(result5==NULL);

    struct parse *result6 = parse_prompt("add view N5 400x400400200456a");
    assert(result6==NULL);
}

void test_del_view(){
    struct parse *result1 = parse_prompt("del view N34");
    assert(result1->size == 3);
    int i=0;
    assert(!strcmp(result1->tab[i], "del"));
    i++;
    assert(!strcmp(result1->tab[i], "view"));
    i++;
    assert(!strcmp(result1->tab[i], "N34"));
    free_parser(result1);

    struct parse *result2 = parse_prompt("del");
    assert(result2==NULL);
    
    struct parse *result3 = parse_prompt("del view");
    assert(result3==NULL);

    struct parse *result4 = parse_prompt("del hello");
    assert(result4==NULL);

    struct parse *result5 = parse_prompt("del view dfghjk");
    assert(result5==NULL);

    struct parse *result6 = parse_prompt("del view N5 N7");
    assert(result6==NULL);

}

void test_save(){
    struct parse *result1 = parse_prompt("save aquarium");
    assert(result1->size == 2);
    int i=0;
    assert(!strcmp(result1->tab[i], "save"));
    i++;
    assert(!strcmp(result1->tab[i], "aquarium"));
    free_parser(result1);

    struct parse *result2 = parse_prompt("save");
    assert(result2==NULL);
    
    struct parse *result3 = parse_prompt("save aquarium aquarim2");
    assert(result3==NULL);

    struct parse *result4 = parse_prompt("save (è-_è_)");
    assert(result4==NULL);

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