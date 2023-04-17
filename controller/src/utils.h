#ifndef _UTILS_H_
#define _UTILS_H_

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <ctype.h>


enum func { LOAD, SHOW, ADD_VIEW, DEL_VIEW, SAVE, STATUS, ADDFISH, DELFISH, STARTFISH, HELLO, GETFISHES, GFCONTINUOUSLY, LS, PING, LOG, UNKNOWN };

struct parse {
    char **tab; // a list of parsed elements
    int size; // the number of elements(words, numbers...) in the tab
    enum func func_name;
};

void exit_if(int condition, const char *prefix);
struct parse *parse_prompt(char *str);
struct parse *parse_clients(char *str);

#endif // _UTILS_H_