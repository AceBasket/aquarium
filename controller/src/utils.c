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
        free(p->tab[i]);
    }
    free(p->tab);
    free(p);
}
void adding_arg_to_parse(struct parse *p, char *arg) {
    p->tab = realloc(p->tab, sizeof(char *) * (p->size + 1));
    p->tab[p->size] = malloc(strlen(arg) + 1);
    strcpy(p->tab[p->size], arg);
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
    printf("too much arguments!\n");
    printf("the commande should be like : %s\n", str);
    free_parser(p);
}

void too_few_arguments(struct parse *p, char *str) {
    printf("too few arguments!\n");
    printf("the commande should be like : %s\n", str);
    free_parser(p);
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

    char *command;
    command = strtok(string, " ");
    if (command == NULL) {
        printf("No command provided.\n");
        return NULL;
    }

    struct parse *p = malloc(sizeof(*p));
    p->size = 0;
    p->tab = malloc(sizeof(char *));
    p->tab[p->size] = malloc(strlen(command) + 1);
    strcpy(p->tab[p->size], command);
    p->size++;

    // load 
    if (strcmp(command, "load") == 0) {
        p->func_name = LOAD;
        char *arg = strtok(NULL, " ");
        if (arg == NULL || arg == 0) {
            too_few_arguments(p, "load <aquarium name>");
            return NULL;
        }

        adding_arg_to_parse(p, arg);
        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "load <aquaruim name>");
            return NULL;
        }
        return p;
    }
    //show aquarium
    else if (strcmp(command, "show") == 0) {
        p->func_name = SHOW;
        char *arg = strtok(NULL, " ");
        if (arg == NULL) {
            too_few_arguments(p, "show <aquarium name>");
            return NULL;
        }

        adding_arg_to_parse(p, arg);

        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "load show <aquaruim name>");
            return NULL;
        }
        return p;
    }

    //add view
    else if (strcmp(command, "add") == 0) {
        p->func_name = ADD_VIEW;
        char *command2 = strtok(NULL, " ");
        if (command2 == NULL) {
            too_few_arguments(p, "add view N5 400x400+400+200");
            return NULL;
        }
        if (strcmp(command2, "view") != 0) {
            printf("Wrong command\n");
            printf("the command should be like: add view N5 400x400+400+200\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, command2);

        char *arg1 = strtok(NULL, " ");
        if (arg1 == NULL) {
            printf("too fiew arguments! \n");
            printf("the command should be like: add view N5 400x400+400+200\n");
            free_parser(p);
            return NULL;
        } else if (strlen(arg1) < 2 || !isalpha(arg1[0]) || !is_number(arg1, 1)) {
            printf("The name of the view should be like: N<number>\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg1);

        char *arg2 = strtok(NULL, "x");
        if (arg2 == NULL) {
            printf("too fiew arguments! \n");
            printf("the command should be like: add view N5 400x400+400+200\n");
            free_parser(p);
            return NULL;
        } else if (!is_number(arg2, 0)) {
            printf("The 1st cordinate of the view should be an integer: <number>\nInstead you gave the following :%s\n", arg2);
            printf("or the separation between 1st and 2nd cordinate sould be an x (times) symbol\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg2);

        char *arg3 = strtok(NULL, "+");
        if (arg2 == NULL) {
            printf("too fiew arguments! \n");
            printf("the command should be like: add view N5 400x400+400+200\n");
            free_parser(p);
            return NULL;
        } else if (!is_number(arg3, 0)) {
            printf("The 2nd cordinate of the view should be an integer: <number>\n");
            printf("Or the separation between 2nd and 3rd cordinate should be a + (plus) symbol\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg3);

        char *arg4 = strtok(NULL, "+");
        if (arg4 == NULL) {
            printf("too fiew arguments! \n");
            printf("the command should be like: add view N5 400x400+400+200\n");
            free_parser(p);
            return NULL;
        } else if (!is_number(arg4, 0)) {
            printf("The 3rd cordinate of the view should be an integer: <number>\n");
            printf("Or the separation between the 3rd and forth argument should be a + (plus) symbol");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg4);

        char *arg5 = strtok(NULL, "");
        if (arg5 == NULL) {
            printf("too fiew arguments! \n");
            printf("the command should be like: add view N5 400x400+400+200\n");
            free_parser(p);
            return NULL;
        } else if (!is_number(arg5, 0)) {
            printf("The 4th cordinate of the view must be an integer: <number>\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg5);
        if (strtok(NULL, "") != NULL) {
            too_much_arguments(p, "add view N5 400x400+400+200");
            return NULL;
        }
        return p;
    }
    // del view
    else if (strcmp(command, "del") == 0) {
        p->func_name = DEL_VIEW;
        char *command2 = strtok(NULL, " ");
        if (command2 == NULL) {
            too_few_arguments(p, "del view <view name>");
            return NULL;
        }
        if (strcmp(command2, "view") != 0) {
            printf("Wrong command\n");
            printf("the command should be like: del view <view name>\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, command2);

        char *arg1 = strtok(NULL, " ");
        if (arg1 == NULL) {
            too_few_arguments(p, "del view <view name>");
            return NULL;
        } else if (strlen(arg1) < 2 || !isalpha(arg1[0]) || !is_number(arg1, 1)) {
            printf("The name of the view should be like: N<number>\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg1);

        if (strtok(NULL, "") != NULL) {
            too_much_arguments(p, "del view <view name>");
            return NULL;
        }
        return p;
    }
    //save
    else if (strcmp(command, "save") == 0) {
        p->func_name = SAVE;
        char *arg = strtok(NULL, " ");
        if (arg == NULL) {
            too_few_arguments(p, "save <aquarium name>");
            return NULL;
        }
        else if (!is_alphanum(arg)){
            printf("the name of the aquarium should be composed of numbers and/or alphabets\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg);

        if (strtok(NULL, "") != NULL) {
            too_much_arguments(p, "save <aquarium name>");
            return NULL;
        }
        return p;
    }
    return NULL;
}

struct parse *parse_clients(char *str) {

    char string[strlen(str)];
    strcpy(string, str);

    char *command;
    command = strtok(string, " ");
    if (command == NULL) {
        printf("No command provided.\n");
        return NULL;
    }

    struct parse *p = malloc(sizeof(*p));
    p->size = 0;
    p->tab = malloc(sizeof(char *));
    p->tab[p->size] = malloc(strlen(command) + 1);
    strcpy(p->tab[p->size], command);
    p->size+=1;

    //status
    if (strcmp(command, "status") == 0) {
        p->func_name = STATUS;
        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "status");
            return NULL;
        }
        return p;
    }
    //addFish
    else if (strcmp(command, "addFish") == 0) {
        p->func_name = ADDFISH;
        char *arg1 = strtok(NULL, " ");
        if (arg1 == NULL) {
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>>");
            return NULL;
        }
        if (!is_alphanum(arg1)) {
            printf("the name of the fish should only contrain alphabets and/or numbers");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg1);

        char *command2 = strtok(NULL, " ");
        if (command2 == NULL) {
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return NULL;
        }
        if (strcmp(command2, "at") != 0) {
            printf("Wrong command\n");
            printf("command should be like : addFish <nameFish> at <number>x<number>, <number>x<number>, <path>\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, command2);

        char *arg2 = strtok(NULL, "x");
        if (arg2 == NULL) {
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return NULL;
        }
        if (!is_number(arg2, 0)) {
            printf("the first coord should be a number\n");
            printf("Or the separation between the 1st argument and 2nd should be an <x> symbol\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg2);

        char *arg3 = strtok(NULL, ", ");
        if (arg3 == NULL) {
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return NULL;
        }
        if (!is_number(arg3, 0)) {
            printf("the second(%s) coord should be a number\n", arg3);
            printf("Or the separation between the 2nd argument and 3rd should be an <, > symbol\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg3);

        char *arg4 = strtok(NULL, "x");

        if (arg4 == NULL) {
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return NULL;
        }
        if (!is_number(arg4, 1)) {
            printf("the 4th(%s) coord should be a number\n", arg4);
            printf("Or the separation between the 3nd argument and 4th should be an <x> symbol\n");
            free_parser(p);
            return NULL;
        }

        adding_arg_to_parse(p, arg4);

        char *arg5 = strtok(NULL, ",");
        if (arg5 == NULL) {
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return NULL;
        }
        if (!is_number(arg5, 0)) {
            printf("the 5th(%s) coord should be a number\n", arg5);
            printf("Or the separation between the 4th argument and 5th should be an <, > symbol\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg5);

        char *arg6 = strtok(NULL, " ");

        if (arg6 == NULL) {
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return NULL;
        }
        if (!is_alphanum(arg6)) {
            printf("the path(%s) name should be composed of alphabets and/or numbers\n", arg6);
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg6);
        if (strtok(NULL, "") != NULL) {
            too_much_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return NULL;
        }
        return p;
    }
    //delFish
    else if (strcmp(command, "delFish") == 0) {
        p->func_name = DELFISH;
        char *arg = strtok(NULL, " ");
        if (arg == NULL) {
            too_few_arguments(p, "delFish <nameFish>");
            return NULL;
        } else if (!is_alphanum(arg)) {
            printf("the name of the fish should be composed of aplphabets and/or numbers");
            free_parser(p);
            return NULL;
        }
        if (strtok(NULL, "") != NULL) {
            too_much_arguments(p, "delFish <nameFish>");
            return NULL;
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
            return NULL;
        } else if (!is_alphanum(arg)) {
            printf("the name of the fish should be composed of aplphabets and/or numbers");
            free_parser(p);
            return NULL;
        }
        if (strtok(NULL, "") != NULL) {
            too_much_arguments(p, "startFish <nameFish>");
            return NULL;
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
            printf("Command should be like : hello in as <ID> Or a simple hello\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, command);

        command = strtok(NULL, " ");
        if (command == NULL || strcmp(command, "as") != 0) {
            printf("Command should be like: hello in as <ID> or a simple hello\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, command);

        char *ID = strtok(NULL, " ");
        if (ID == NULL) {
            too_few_arguments(p, "hello in as <ID> or a simple hello");
            return NULL;
        } else if (strlen(ID) < 2 || ID[0]!='N' || !is_number(ID, 1)) {
            printf("The name of the ID should be like: N<number>\n");
            free_parser(p);
            return NULL;
        }
         else if (strtok(NULL, "") != NULL) {
            too_much_arguments(p, "hello in as <ID> or a simple hello");
            return NULL;
        }
        adding_arg_to_parse(p, ID);
        return p;
    }
    //getFishes
    else if (strcmp(command, "getFishes") == 0) {
        p->func_name = GETFISHES;
        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "getFishes");
            return NULL;
        }
        return p;
    }
    //ls
    else if (strcmp(command, "ls") == 0) {
        p->func_name = LS;
        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "ls");
            return NULL;
        }
        return p;
    }
    //getFishesContinuously
    else if (strcmp(command, "getFishesContinuously") == 0) {
        p->func_name = GFCONTINUOUSLY;
        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "getFishesContinuously");
            return NULL;
        }
        return p;
    }
    //ping
    else if (strcmp(command, "ping") == 0) {
        p->func_name = PING;
        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "ping");
            return NULL;
        }
        return p;
    }
    //log out
    else if (strcmp(command, "log") == 0) {
        p->func_name = LOG;
        command = strtok(NULL, " ");
        if (command == NULL || strcmp(command, "out")) {
            printf("Command should be like: log out\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, command);
        if (strtok(NULL, " ") != NULL) {
            too_much_arguments(p, "log out");
            return NULL;
        }
        return p;
    } else {
        printf("unknown command\n");
        p->func_name = UNKNOWN;
    }
    free_parser(p);
    return NULL;
}