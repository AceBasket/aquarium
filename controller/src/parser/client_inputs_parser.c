#include "client_inputs_parser.h"

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
        // adding_arg_to_parse(p, command2);

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

        char *arg4 = strtok(NULL, "x ");

        if (arg4 == NULL) {
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return p;
        }
        if (!is_number(arg4, 1)) {
            sprintf(p->status, "ERROR: the 4th(%s) coord should be a number@@Or the separation between the 3nd argument and 4th should be an <x> symbol\n", arg4);
            return p;
        }

        adding_arg_to_parse(p, arg4);

        char *arg5 = strtok(NULL, ", ");
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
        // adding_arg_to_parse(p, command);

        command = strtok(NULL, " ");
        if (command == NULL || strcmp(command, "as") != 0) {
            strcpy(p->status, "ERROR: Command should be like: hello in as <ID> or a simple hello\n");
            return p;
        }
        // adding_arg_to_parse(p, command);

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
        char *id = strtok(NULL, " ");
        if (id == NULL) {
            too_few_arguments(p, "ping <ID>");
            return p;
        } else if (!is_number(id, 0)) {
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