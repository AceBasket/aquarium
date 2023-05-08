#include "parser.h"


void free_parser(struct parse *p) {
    for (int i = 0; i < p->size; i++) {
        free(p->arguments[i]);
    }
    free(p->status);
    free(p->arguments);
    free(p);
}

void adding_arg_to_parse(struct parse *p, char *arg) {
    p->arguments = realloc(p->arguments, sizeof(char *) * (p->size + 1));
    p->arguments[p->size] = malloc(strlen(arg) + 1);
    strcpy(p->arguments[p->size], arg);
    p->size++;
}

int is_number(char *str, int start) {
    int len = strlen(str);
    for (int i = start; i < len; i++) {
        if (!isdigit(str[i])) {
            return 0; //if any non-digit character is found
        }
    }
    return 1; // Return 1 if all characters are digits
}

int is_alphanum(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (!isalnum(str[i])) {
            return 0;
        }
    }
    return 1;
}

void too_much_arguments(struct parse *p, char *str) {
    sprintf(p->status, "ERROR: too much arguments!@@the commande should be like : %s\n", str);
}

void too_few_arguments(struct parse *p, char *str) {
    sprintf(p->status, "ERROR: too few arguments!@@the commande should be like : %s\n", str);
}

char *remove_spaces(char *str) {
    //int c = n_spaces(str);
    char *string = malloc(sizeof(char) * strlen(str));
    strcpy(string, str);
    int i;
    int j = -1;
    for (i = 0; string[i]; i++)
        if (string[i] != ' ')
            string[++j] = string[i];
    string[++j] = '\0';
    return string;
}
