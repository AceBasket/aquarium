#include "utils.h"

void exit_if(int condition, const char *prefix) {
    if (condition) {
        if (errno != 0) {
            perror(prefix);
        } else {
            fprintf(stderr, "%s\n", prefix);
        }
        exit(1);
    }
}

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
    //printf("%s\n", string);
    return string;
}

struct parse *parse_prompt(char *str) {

    char string[strlen(str)];
    strcpy(string, str);

    struct parse *p = malloc(sizeof(*p));
    p->status = malloc(sizeof(char) * 200);
    strcpy(p->status, "OK\n");
    p->size = 0;
    p->arguments = malloc(sizeof(char *));

    char *command;
    command = strtok(string, " ");
    if (command == NULL) {
        strcpy(p->status, "ERROR: No command provided.\n");
        p->arguments[p->size] = malloc(1); // to avoid segfault when freeing
        return p;
    }

    // load 
    if (strcmp(command, "load") == 0) {
        p->func_name = LOAD;
        char *arg = strtok(NULL, " ");
        if (arg == NULL || arg == 0) {
            too_few_arguments(p, "load <aquarium name>");
            return p;
        }

        adding_arg_to_parse(p, arg);
        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "load <aquaruim name>");
            return p;
        }
        return p;
    }
    //show aquarium
    else if (strcmp(command, "show") == 0) {
        p->func_name = SHOW;
        char *arg = strtok(NULL, " ");
        if (arg == NULL) {
            too_few_arguments(p, "show <aquarium name>");
            return p;
        }

        adding_arg_to_parse(p, arg);

        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "load show <aquaruim name>");
            return p;
        }
        return p;
    }

    //add view
    else if (strcmp(command, "add") == 0) {
        p->func_name = ADD_VIEW;
        char *command2 = strtok(NULL, " ");
        if (command2 == NULL) {
            too_few_arguments(p, "add view N5 400x400+400+200");
            return p;
        }
        if (strcmp(command2, "view") != 0) {
            strcpy(p->status, "ERROR: Wrong command@@the command should be like: add view N5 400x400+400+200\n");
            return p;
        }
        adding_arg_to_parse(p, command2);

        char *arg1 = strtok(NULL, " ");
        if (arg1 == NULL) {
            too_few_arguments(p, "add view N5 400x400+400+200");
            return p;
        } else if (strlen(arg1) < 2 || !isalpha(arg1[0]) || !is_number(arg1, 1)) {
            strcpy(p->status, "ERROR: The name of the view should be like: N<number>\n");
            return p;
        }
        adding_arg_to_parse(p, arg1);

        char *arg2 = strtok(NULL, "x");
        if (arg2 == NULL) {
            too_few_arguments(p, "add view N5 400x400+400+200");
            return p;
        } else if (!is_number(arg2, 0)) {
            sprintf(p->status, "ERROR: The 1st cordinate of the view should be an integer: <number>@@Instead you gave the following :%s@@or the separation between 1st and 2nd cordinate sould be an x (times) symbol\n", arg2);
            return p;
        }
        adding_arg_to_parse(p, arg2);

        char *arg3 = strtok(NULL, "+");
        if (arg2 == NULL) {
            too_few_arguments(p, "add view N5 400x400+400+200");
            return p;
        } else if (!is_number(arg3, 0)) {
            strcpy(p->status, "ERROR: The 2nd cordinate of the view should be an integer: <number>@@Or the separation between 2nd and 3rd cordinate should be a + (plus) symbol\n");
            return p;
        }
        adding_arg_to_parse(p, arg3);

        char *arg4 = strtok(NULL, "+");
        if (arg4 == NULL) {
            too_few_arguments(p, "add view N5 400x400+400+200");
            return p;
        } else if (!is_number(arg4, 0)) {
            strcpy(p->status, "ERROR: The 3rd cordinate of the view should be an integer: <number>@@Or the separation between the 3rd and forth argument should be a + (plus) symbol");
            return p;
        }
        adding_arg_to_parse(p, arg4);

        char *arg5 = strtok(NULL, "");
        if (arg5 == NULL) {
            too_few_arguments(p, "add view N5 400x400+400+200");
            return p;
        } else if (!is_number(arg5, 0)) {
            strcpy(p->status, "ERROR: The 4th cordinate of the view must be an integer: <number>\n");
            return p;
        }
        adding_arg_to_parse(p, arg5);
        if (strtok(NULL, "") != NULL) {
            too_much_arguments(p, "add view N5 400x400+400+200");
            return p;
        }
        return p;
    }
    // del view
    else if (strcmp(command, "del") == 0) {
        p->func_name = DEL_VIEW;
        char *command2 = strtok(NULL, " ");
        if (command2 == NULL) {
            too_few_arguments(p, "del view <view name>");
            return p;
        }
        if (strcmp(command2, "view") != 0) {
            strcpy(p->status, "ERROR: Wrong command@@the command should be like: del view <view name>\n");
            return p;
        }
        adding_arg_to_parse(p, command2);

        char *arg1 = strtok(NULL, " ");
        if (arg1 == NULL) {
            too_few_arguments(p, "del view <view name>");
            return p;
        } else if (strlen(arg1) < 2 || !isalpha(arg1[0]) || !is_number(arg1, 1)) {
            strcpy(p->status, "ERROR: The name of the view should be like: N<number>\n");
            return p;
        }
        adding_arg_to_parse(p, arg1);

        if (strtok(NULL, "") != NULL) {
            too_much_arguments(p, "del view <view name>");
            return p;
        }
        return p;
    }
    //save
    else if (strcmp(command, "save") == 0) {
        p->func_name = SAVE;
        char *arg = strtok(NULL, " ");
        if (arg == NULL) {
            too_few_arguments(p, "save <aquarium name>");
            return p;
        } // else if (!is_alphanum(arg)) {
        //     strcpy(p->status, "ERROR: the name of the aquarium should be composed of numbers and/or alphabets\n");
        //     return p;
        // }
        adding_arg_to_parse(p, arg);

        if (strtok(NULL, "") != NULL) {
            too_much_arguments(p, "save <aquarium name>");
            return p;
        }
        return p;
    } else {
        strcpy(p->status, "ERROR: Unknown command\n");
        p->func_name = UNKNOWN;
        return p;
    }
}

struct parse *parse_file(FILE *f) {
    size_t size = 100;
    char *line = malloc(size * sizeof(char));
    ssize_t read = 0;

    struct parse *p = malloc(sizeof(*p));
    p->status = malloc(sizeof(char) * 200);
    strcpy(p->status, "OK\n");
    // p->size = 1;
    // p->arguments = malloc(sizeof(char *));

    while (read != -1) {
        read = getline(&line, &size, f);
        if (read == -1) {
            break;
        }
        for (size_t i = 0; i < size; i++) {
            if (line[i] == '\n') {
                line[i] = '\0';
            }
        }

        if (isdigit(line[0])) {
            /* TODO check that the whole "word" is a number (not just the beginning) */
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

        else if (isalpha(line[0])) {
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

    return p;
}

struct parse *parse_clients(char *str) {

    char string[strlen(str)];
    strcpy(string, str);

    struct parse *p = malloc(sizeof(*p));
    p->status = malloc(sizeof(char) * 200);
    strcpy(p->status, "OK\n");
    p->size = 0;
    p->arguments = malloc(sizeof(char *));

    char *command;
    command = strtok(string, " ");
    if (command == NULL) {
        strcpy(p->status, "No command provided.\n");
        p->arguments[p->size] = malloc(1); // to avoid segfault when freeing
        return p;
    }

    //status
    if (strcmp(command, "status") == 0) {
        p->func_name = STATUS;
        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "status");
        }
        return p;
    }
    //addFish
    else if (strcmp(command, "addFish") == 0) {
        p->func_name = ADDFISH;
        char *arg1 = strtok(NULL, " ");
        if (arg1 == NULL) {
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <pathwayModel>>");
            return p;
        }
        if (!is_alphanum(arg1)) {
            strcpy(p->status, "ERROR: the name of the fish should only contain alphanums");
            return p;
        }
        adding_arg_to_parse(p, arg1);

        char *command2 = strtok(NULL, " ");
        if (command2 == NULL) {
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return p;
        }
        if (strcmp(command2, "at") != 0) {
            strcpy(p->status, "ERROR: Wrong command@@command should be like : addFish <nameFish> at <number>x<number>, <number>x<number>, <path>\n");
            return p;
        }
        adding_arg_to_parse(p, command2);

        char *arg2 = strtok(NULL, "x");
        if (arg2 == NULL) {
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return p;
        }
        if (!is_number(arg2, 0)) {
            strcpy(p->status, "ERROR: the first coord should be a number@@Or the separation between the 1st argument and 2nd should be an <x> symbol\n");
            return p;
        }
        adding_arg_to_parse(p, arg2);

        char *arg3 = strtok(NULL, ", ");
        if (arg3 == NULL) {
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return p;
        }
        if (!is_number(arg3, 0)) {
            sprintf(p->status, "ERROR: the second(%s) coord should be a number@@Or the separation between the 2nd argument and 3rd should be an <, > symbol\n", arg3);
            return p;
        }
        adding_arg_to_parse(p, arg3);

        char *arg4 = strtok(NULL, "x");

        if (arg4 == NULL) {
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return p;
        }
        if (!is_number(arg4, 1)) {
            sprintf(p->status, "ERROR: the 4th(%s) coord should be a number@@Or the separation between the 3nd argument and 4th should be an <x> symbol\n", arg4);
            return p;
        }

        adding_arg_to_parse(p, arg4);

        char *arg5 = strtok(NULL, ",");
        if (arg5 == NULL) {
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return p;
        }
        if (!is_number(arg5, 0)) {
            sprintf(p->status, "ERROR: the 5th(%s) coordinate should be a number@@Or the separation between the 4th argument and 5th should be an <, > symbol\n", arg5);
            return p;
        }
        adding_arg_to_parse(p, arg5);

        char *arg6 = strtok(NULL, " ");

        if (arg6 == NULL) {
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return p;
        }
        if (!is_alphanum(arg6)) {
            sprintf(p->status, "ERROR: the path(%s) name should be composed of alphanums\n", arg6);
            return p;
        }
        adding_arg_to_parse(p, arg6);
        if (strtok(NULL, "") != NULL) {
            too_much_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return p;
        }
        return p;
    }
    //delFish
    else if (strcmp(command, "delFish") == 0) {
        p->func_name = DELFISH;
        char *arg = strtok(NULL, " ");
        if (arg == NULL) {
            too_few_arguments(p, "delFish <nameFish>");
            return p;
        } else if (!is_alphanum(arg)) {
            strcpy(p->status, "ERROR: the name of the fish should be composed of alphanums\n");
            return p;
        }
        if (strtok(NULL, "") != NULL) {
            too_much_arguments(p, "delFish <nameFish>");
            return p;
        }
        adding_arg_to_parse(p, arg);
        return p;
    }
    //startFish
    else if (strcmp(command, "startFish") == 0) {
        p->func_name = STARTFISH;

        char *arg = strtok(NULL, " ");
        if (arg == NULL) {
            too_few_arguments(p, "startFish <nameFish>");
            return p;
        } else if (!is_alphanum(arg)) {
            strcpy(p->status, "ERROR: the name of the fish should be composed of alphanums\n");
            return p;
        }
        if (strtok(NULL, "") != NULL) {
            too_much_arguments(p, "startFish <nameFish>");
            return p;
        }
        adding_arg_to_parse(p, arg);
        return p;
    }
    //hello
    else if (strcmp(command, "hello") == 0) {
        p->func_name = HELLO;

        command = strtok(NULL, " ");
        if (command == NULL) {
            return p;
        }

        if (strcmp(command, "in") != 0) {
            strcpy(p->status, "ERROR: Command should be like : hello in as <ID> Or a simple hello\n");
            return p;
        }
        adding_arg_to_parse(p, command);

        command = strtok(NULL, " ");
        if (command == NULL || strcmp(command, "as") != 0) {
            strcpy(p->status, "ERROR: Command should be like: hello in as <ID> or a simple hello\n");
            return p;
        }
        adding_arg_to_parse(p, command);

        char *ID = strtok(NULL, " ");
        if (ID == NULL) {
            too_few_arguments(p, "hello in as <ID> or a simple hello");
            return p;
        } else if (strlen(ID) < 2 || ID[0] != 'N' || !is_number(ID, 1)) {
            strcpy(p->status, "ERROR: The name of the ID should be like: N<number>\n");
            return p;
        } else if (strtok(NULL, "") != NULL) {
            too_much_arguments(p, "hello in as <ID> or a simple hello");
            return p;
        }
        adding_arg_to_parse(p, ID);
        return p;
    }
    //getFishes
    else if (strcmp(command, "getFishes") == 0) {
        p->func_name = GETFISHES;
        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "getFishes");
            return p;
        }
        return p;
    }
    //ls
    else if (strcmp(command, "ls") == 0) {
        p->func_name = LS;
        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "ls");
            return p;
        }
        return p;
    }
    //getFishesContinuously
    else if (strcmp(command, "getFishesContinuously") == 0) {
        p->func_name = GFCONTINUOUSLY;
        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "getFishesContinuously");
            return p;
        }
        return p;
    }
    //ping
    else if (strcmp(command, "ping") == 0) {
        p->func_name = PING;
        char * id = strtok(NULL, " ");
        if ( id == NULL) {
            too_few_arguments(p, "ping <ID>");
            return p;
        }
        else if (!is_number(id, 0)){
            strcpy(p->status, "ERROR: The ID should be a number \n");
            return p;
        }
        adding_arg_to_parse(p, id);
        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "ping <ID>");
            return p;
        }
        return p;
    }
    //log out
    else if (strcmp(command, "log") == 0) {
        p->func_name = LOG;
        command = strtok(NULL, " ");
        if (command == NULL || strcmp(command, "out")) {
            strcpy(p->status, "ERROR: Command should be like: log out\n");
            return p;
        }
        adding_arg_to_parse(p, command);
        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "log out");
            return p;
        }
        return p;
    } else {
        strcpy(p->status, "ERROR: unknown command\n");
        p->func_name = UNKNOWN;
        return p;
    }
}
