#include "prompt_parser.h"

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
        // adding_arg_to_parse(p, command2);

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
        // adding_arg_to_parse(p, command2);

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