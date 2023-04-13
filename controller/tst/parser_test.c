#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>


extern struct parse* parser(char *str);

void test_addFish(){
    struct parse *result = parser("addFish PoissonNain at 61x52, 4x3, RandomPathway");
    assert(result->size == 8);
    int i=0;
    assert(!strcmp(result->tab[i], "addFish"));
    assert(!strcmp(result->tab[i++], "PoissonNain"));
    assert(!strcmp(result->tab[i++], "at"));
    assert(!strcmp(result->tab[i++], "61"));


    result = parser("addFish PoissonNain at 61x52, 4x3, ");
    assert(result==NULL);
}

void test_delFish(){
    struct parse *result = parser("delFish fishName");
    assert(result->size == 2);
    int i=0;
    assert(!strcmp(result->tab[i], "delFish"));
    assert(!strcmp(result->tab[i++], "fishName"));

    result = parser("delFish");
    assert(result==NULL);
    
    result = parser("delFish fish1 fish2");
    assert(result==NULL);
}

void test_startFish(){
    struct parse *result = parser("startFish fishName");
    assert(result->size == 2);
    int i=0;
    assert(!strcmp(result->tab[i], "startFish"));
    assert(!strcmp(result->tab[i++], "fishName"));
    
    result = parser("startFish");
    assert(result==NULL);
    
    result = parser("startFish fish1 fish2");
    assert(result==NULL);
}

void test_hello(){
    struct parse *result = parser("hello in as 12");
    assert(result->size == 4);
    int i=0;
    assert(!strcmp(result->tab[i], "hello"));
    assert(!strcmp(result->tab[i++], "in"));
    assert(!strcmp(result->tab[i++], "as"));
    assert(!strcmp(result->tab[i++], "12"));

    result = parser("hello in as 12 HJK");
    assert(result==NULL);

    result = parser("hello in as");
    assert(result==NULL);

    result = parser("hello in as HJK");
    assert(result==NULL);
}

int main(void) {
    
    return 0;
}