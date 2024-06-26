#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <ctype.h>

enum func { LOAD, SHOW, ADD_VIEW, DEL_VIEW, SAVE, STATUS, ADD_FISH, DEL_FISH, START_FISH, HELLO, GET_FISHES, GET_FISHES_CONTINUOUSLY, LS, PING, LOG, UNKNOWN };

struct parse {
    char **arguments; // a list of parsed elements
    int size; // the number of elements(words, numbers...) in the arguments
    enum func func_name;
    char *status;
};

/* frees a parser */
void free_parser(struct parse *p);
/* add the argument in the arguments list from the parser */
void adding_arg_to_parse(struct parse *p, char *arg);
/* tests if str is a number */
int is_number(char *str, int start);
/* tests if str is a mix between numbers and letters */
int is_alphanum(char *str);
/* modifies the parser's status with the error: "too much arguments" */
void too_much_arguments(struct parse *p, char *str);
/* modifies the parser's status with the error: "too few arguments" */
void too_few_arguments(struct parse *p, char *str);
/* removes spaces from str */
char *remove_spaces(char *str);

#endif // _PARSER_H_