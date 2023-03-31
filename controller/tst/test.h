#ifndef __TEST_H__
#define __TEST_H__

// #include "../src/parser.h"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <dlfcn.h>

#define DEF "\033[00m"
#define RD "\x1B[31m"
#define GRN "\x1B[32m"
#define YLW "\x1B[33m"
#define PRPL "\033[0;35m"
#define BLU "\033[0;36m"

#define NO_ERROR 1
#define ERROR 0

#define test_function(name_function,  condition)\
    printf("%sTest on %s...%s", BLU, #name_function, DEF);\
    if (condition)\
        printf("\n  Condition: %s - %sPASSED%s\n\n",#condition, GRN, DEF);\
    else\
        printf("\n  Condition: %s - %s ERROR%s\n",#condition, RD, DEF);

#define check_function(name_function, condition)\
    printf("%sTest on %s...%s", BLU, #name_function, DEF);\
    printf("\n  Check if: %s - %sCHECK%s\n\n",#condition, YLW, DEF);\

extern void parser(char *str);

void fct_test_parse();


#endif // __TEST_H__