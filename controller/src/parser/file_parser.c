#include "file_parser.h"

struct parse *parse_file(FILE *f) {
    size_t size = 100;
    char *line = calloc(size, sizeof(char)); // No uninitialized memory
    ssize_t read = 0;

    struct parse *p = malloc(sizeof(*p));
    p->status = malloc(sizeof(char) * 200);
    strcpy(p->status, "OK\n");
    p->size = 0;
    p->arguments = malloc(sizeof(char *));

    while (read != -1) {
        read = getline(&line, &size, f);
        if (read == -1) {
            break;
        }
        for (size_t i = 0; i < size; i++) {
            if (line[i] == '\n') {
                line[i] = '\0';
                break;
            }
        }

        if (isdigit(line[0])) {
            // aquarium
            char *arg1 = strtok(line, "x");
            if (arg1 == NULL) {
                strcpy(p->status, "ERROR: No 1st argument given\nThe information about the aquarium should be given like: 1000x1000\n");
                return p;
            } else if (!is_number(arg1, 0)) {
                strcpy(p->status, "ERROR: The 1st argument of the aquarium should be an integer: <number>\nOr the separation between 1st and 2nd argument sould be an x (times) symbol\n");
                return p;
            }
            adding_arg_to_parse(p, arg1);

            char *arg2 = strtok(NULL, "");
            if (arg2 == NULL) {
                strcpy(p->status, "ERROR: No 2nd argument given\nThe information about the aquarium should be given like: 1000x1000\n");
                return p;
            } else if (!is_number(arg2, 0)) {
                strcpy(p->status, "ERROR: The 2nd argument of the aquarium must be an integer: <number>\n");
                return p;
            }
            adding_arg_to_parse(p, arg2);

            if (strtok(NULL, "") != NULL) {
                strcpy(p->status, "ERROR: The information about the aquarium should be given like: 1000x1000\n");
                return p;
            }
        }

        else if (isalpha(line[0]) && isalpha(line[1])) {
            // cfg
            char *arg1 = strtok(line, " ");
            if (arg1 == NULL) {
                strcpy(p->status, "ERROR: Incorrect file formatting\n");
                return p;
            } else if (is_number(arg1, 0)) {
                strcpy(p->status, "ERROR: Incorrect variable name\n");
                return p;
            }
            adding_arg_to_parse(p, arg1);

            char *arg2 = strtok(NULL, " ");
            if (arg2 == NULL || strcmp(arg2, "=") != 0) {
                strcpy(p->status, "ERROR: Incorrect file formatting\n");
                return p;
            }

            char *arg3 = strtok(NULL, "");
            if (arg3 == NULL) {
                sprintf(p->status, "ERROR: No value for the variable %s\n", arg1);
                return p;
            } else if (!is_number(arg3, 0)) {
                sprintf(p->status, "ERROR: Incorrect value for the variable %s\n", arg1);
                return p;
            }
            adding_arg_to_parse(p, arg3);

            if (strtok(NULL, "") != NULL) {
                strcpy(p->status, "ERROR: \n");
                return p;
            }
        }

        else if (isalpha(line[0]) && isdigit(line[1])) {
            // view
            char *arg1 = strtok(line, " ");
            if (arg1 == NULL) {
                strcpy(p->status, "ERROR: No view provided\n");
                return p;
            } else if (strlen(arg1) < 2 || !isalpha(arg1[0]) || !is_number(arg1, 1)) {
                strcpy(p->status, "ERROR: No name provided for the view\nThe name of the view should be like: N<number>\n");
                return p;
            }
            adding_arg_to_parse(p, arg1);

            char *arg2 = strtok(NULL, "x");
            if (arg2 == NULL) {
                strcpy(p->status, "ERROR: No 1st argument given\nThe information about the view should be given like: N5 400x400+400+200\n");
                return p;
            } else if (!is_number(arg2, 0)) {
                sprintf(p->status, "ERROR: The 1st argument of the view should be an integer: <number>\nInstead you gave the following :%s\nOr the separation between 1st and 2nd argument sould be an x (times) symbol\n", arg2);
                return p;
            }
            adding_arg_to_parse(p, arg2);

            char *arg3 = strtok(NULL, "+");
            if (arg2 == NULL) {
                strcpy(p->status, "ERROR: No 2nd argument given\nThe information about the view should be given like: N5 400x400+400+200\n");
                return p;
            } else if (!is_number(arg3, 0)) {
                strcpy(p->status, "ERROR: The 2nd argument of the view should be an integer: <number>\nOr the separation between 2nd and 3rd argument should be a + (plus) symbol\n");
                return p;
            }
            adding_arg_to_parse(p, arg3);

            char *arg4 = strtok(NULL, "+");
            if (arg4 == NULL) {
                strcpy(p->status, "ERROR: No 3rd argument given\nThe information about the view should be given like: N5 400x400+400+200\n");
                return p;
            } else if (!is_number(arg4, 0)) {
                strcpy(p->status, "ERROR: The 3rd argument of the view should be an integer: <number>\nOr the separation between the 3rd and 4th argument should be a + (plus) symbol");
                return p;
            }
            adding_arg_to_parse(p, arg4);

            char *arg5 = strtok(NULL, "");
            if (arg5 == NULL) {
                strcpy(p->status, "ERROR: No 4th argument given\nThe information about the view should be given like: N5 400x400+400+200\n");
                return p;
            } else if (!is_number(arg5, 0)) {
                strcpy(p->status, "ERROR: The 4th argument of the view must be an integer: <number>\n");
                return p;
            }
            adding_arg_to_parse(p, arg5);

            if (strtok(NULL, "") != NULL) {
                strcpy(p->status, "ERROR: The information about the view should be given like: N5 400x400+400+200\n");
                return p;
            }
        }
    }
    free(line);
    return p;
}
