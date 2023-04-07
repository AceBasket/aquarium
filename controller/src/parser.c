#include <string.h>
#include<stdio.h>
#include <stdlib.h>
#include "utils/utils.h"
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>


struct parse{
    char **tab; // a list of parsed elements
    int size; // the number of elements(words, numbers...) in the tab
};

void free_parser(struct parse *p){
    for (int i =0; i< p->size; i++){
        free(p->tab[i]);
    }
    free(p->tab);
    free(p);
}
void adding_arg_to_parse(struct parse *p, char * arg){
    p->tab = realloc(p->tab, sizeof(char*) * (p->size + 1));
    p->tab[p->size] = malloc(strlen(arg) + 1);
    strcpy(p->tab[p->size], arg);
    p->size++;
}

int is_number(char* str, int start) {
    int len = strlen(str);
    for (int i = start; i < len; i++) {
        if (!isdigit(str[i])) {
            return 0; //if any non-digit character is found
        }
    }
    return 1; // Return 1 if all characters are digits
}

int is_alphanum(char *str ){
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (!isalnum(str[i])) {
            return 0;
        }
    }
    return 1;
}

void too_much_arguments(struct parse *p, char * str){
    printf("too much arguments!\n");
    printf("the commande should be like : %s\n", str);
    free_parser(p);
}

void too_few_arguments(struct parse *p, char * str){
    printf("too few arguments!\n");
    printf("the commande should be like : %s\n", str);
    free_parser(p);
}

struct parse * parser(char *str) {
    char *saveptr;

    char string[strlen(str)];
    strcpy(string, str);

    char *command;
    //command = strtok(string, " ");
    command = strtok_r(string, " ", &saveptr);
    if(command == NULL){
        printf("No command provided.\n");
        return NULL;
    }

    struct parse *p=malloc(sizeof(*p));
    p->size =0;
    p->tab = malloc(sizeof(char*));
    p->tab[p->size] = malloc(strlen(command) + 1);
    strcpy(p->tab[p->size], command);
    p->size++;

    // load 
    if (strcmp(command, "load") == 0) {
        char *arg = strtok(NULL, " ");
        if(arg==NULL || arg==0){
            // printf("too fiew arguments! \n");
            // printf("the command should be like: load <aquarium name>");
            // free_parser(p);
            too_few_arguments(p, "load <aquarium name>");
            return NULL;
        }

        p->tab = realloc(p->tab, sizeof(char*) * (p->size + 1));
        p->tab[p->size] = malloc(strlen(command) + strlen(arg) + 1);
        strcpy(p->tab[p->size], arg);
        p->size++;
        if (strtok(NULL, " ") != NULL){
            too_much_arguments(p, "load <aquaruim name>");
            return NULL;
        }
        return p;
    }
    //show aquarium
    else if (strcmp(command, "show") == 0) {
        char *arg = strtok(NULL, " ");
        if(arg == NULL){
            // printf("too fiew arguments! \n");
            // printf("the command should be like: show <aquarium name>\n");
            // free_parser(p);
            too_few_arguments(p, "show <aquarium name>");
            return NULL;
        }
        p->tab = realloc(p->tab, sizeof(char*) * (p->size + 1));
        p->tab[p->size] = malloc(strlen(command) + strlen(arg) + 1);
        strcpy(p->tab[p->size], arg);
        p->size++;
        if (strtok(NULL, " ") != NULL){
            too_much_arguments(p, "load show <aquaruim name>");
            return NULL;
        }
        printf("show %s\n", arg);
        return p;
    }
    //add view
    else if (strcmp(command, "add") == 0) {
        char * command2 = strtok(NULL, " ");
        if(command2 == NULL){
            // printf("too fiew arguments! \n");
            // printf("the command should be like: add view N5 400x400+400+200\n");
            // free_parser(p);
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
        if(arg1 == NULL){
            printf("too fiew arguments! \n");
            printf("the command should be like: add view N5 400x400+400+200\n");
            free_parser(p);
            return NULL;
        }
        else if (strlen(arg1) < 2 || !isalpha(arg1[0]) || !is_number(arg1, 1)){
            printf("The name of the view should be like: N<number>\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg1);

        char *arg2 = strtok(NULL, "x");
        if(arg2 == NULL){
            printf("too fiew arguments! \n");
            printf("the command should be like: add view N5 400x400+400+200\n");
            free_parser(p);
            return NULL;
        }
        else if (!is_number(arg2, 0)){
            printf("The 1st cordinate of the view should be an integer: <number>\nInstead you gave the following :%s\n", arg2);
            printf("or the separation between 1st and 2nd cordinate sould be an x (times) symbol\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg2);

        char *arg3 = strtok(NULL, "+");
        if(arg2 == NULL){
            printf("too fiew arguments! \n");
            printf("the command should be like: add view N5 400x400+400+200\n");
            free_parser(p);
            return NULL;
        }
        else if (!is_number(arg3, 0)){
            printf("The 2nd cordinate of the view should be an integer: <number>\n");
            printf("Or the separation between 2nd and 3rd cordinate should be a + (plus) symbol\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg3);

        char *arg4 = strtok(NULL, "+");
        if(arg4 == NULL){
            printf("too fiew arguments! \n");
            printf("the command should be like: add view N5 400x400+400+200\n");
            free_parser(p);
            return NULL;
        }
        else if (!is_number(arg4, 0)){
            printf("The 3rd cordinate of the view should be an integer: <number>\n");
            printf("Or the separation between the 3rd and forth argument should be a + (plus) symbol");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg4);

        char *arg5 = strtok(NULL, "");
        if(arg5 == NULL){
            printf("too fiew arguments! \n");
            printf("the command should be like: add view N5 400x400+400+200\n");
            free_parser(p);
            return NULL;
        }
        else if (!is_number(arg5, 0)){
            printf("The 4th cordinate of the view must be an integer: <number>\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg5);
        if (strtok(NULL, "") !=NULL){
            too_much_arguments(p, "add view N5 400x400+400+200");
            return NULL;
        }
        return p;
    }
    // del view
    else if (strcmp(command, "del") == 0) {
        char * command2 = strtok(NULL, " ");
        if(command2 == NULL){
            // printf("too fiew arguments! \n");
            // printf("the command should be like: del view <view name>\n");
            // free_parser(p);
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
        if(arg1 == NULL){
            // printf("too fiew arguments! \n");
            // printf("the command should be like: del view <view name>\n");
            // free_parser(p);
            too_few_arguments(p, "del view <view name>");
            return NULL;
        }
        else if (strlen(arg1) < 2 || !isalpha(arg1[0]) || !is_number(arg1, 1)){
            printf("The name of the view should be like: N<number>\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg1); 

        if (strtok(NULL, "") !=NULL){
            too_much_arguments(p, "del view <view name>");
            return NULL;
        }
        return p;
    }
    //save
    else if (strcmp(command, "save") == 0) {
        char *arg = strtok(NULL, " ");
        if(arg == NULL){
            too_few_arguments(p, "save <aquarium name>");
            return NULL;
        }

        adding_arg_to_parse(p, arg);

        if (strtok(NULL, "") !=NULL){
            too_much_arguments(p, "save <aquarium name>");
            return NULL;
        }
        return p;
    }
    //status
    else if (strcmp(command, "status") == 0) {
        if (strtok(NULL, " ") != NULL){
            too_much_arguments(p, "status");
        }
        return p;
    }
    //addFish
    else if (strcmp(command, "addFish") == 0) {
        char *arg1 = strtok_r(NULL, " ", &saveptr);
        if(arg1 == NULL){
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>>");
            return NULL;
        }
        if(!is_alphanum(arg1)){
            printf("the name of the fish should only contrain alphabets and/or numbers");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg1);

        char * command2 = strtok_r(NULL, " ", &saveptr);
        if(command2 == NULL){
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

        char *arg2 = strtok_r(NULL, "x", &saveptr);
        if(arg2 == NULL){
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return NULL;
        }
        if(!is_number(arg2, 0)){
            printf("the first coord should be a number\n");
            printf("Or the separation between the 1st argument and 2nd should be an <x> symbol\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg2);

        char *arg3 = strtok_r(NULL, ",", &saveptr);
        //char *arg3_bis = strtok(arg3, " ");
        if(arg3 == NULL){
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return NULL;
        }
        if(!is_number(arg3, 0)){
            printf("the second(%s) coord should be a number\n", arg3);
            printf("Or the separation between the 2nd argument and 3rd should be an <, > symbol\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg3);

        char *arg4 = strtok_r(NULL, "x", &saveptr);

        if(arg4 == NULL){
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return NULL;
        }
        if(!is_number(arg4, 0)){
            printf("the 4th(%s) coord should be a number\n", arg4);
            printf("Or the separation between the 3nd argument and 4th should be an <x> symbol\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg4);

        char *arg5 = strtok_r(NULL, ", ", &saveptr);
        if(arg5 == NULL){
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return NULL;
        }
        if(!is_number(arg5, 0)){
            printf("the 5th(%s) coord should be a number\n", arg5);
            printf("Or the separation between the 4th argument and 5th should be an <, > symbol\n");
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg5);

        char *arg6 = strtok_r(NULL, "", &saveptr);
        if(arg6 == NULL){
            too_few_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
            return NULL;
        }
        if(!is_alphanum(arg6)){
            printf("the path(%s) name should be composed of alphabets and/or numbers\n", arg6);
            free_parser(p);
            return NULL;
        }
        adding_arg_to_parse(p, arg6);

        if(strtok_r(NULL, "", &saveptr) != NULL){
            too_much_arguments(p, "addFish <nameFish> at <number>x<number>, <number>x<number>, <path>");
        }
        return NULL;
    }
    // else if (strcmp(command, "delFish") == 0) {
    //     char *arg = strtok(NULL, "");
    //     printf("delFish %s\n", arg);
    //     return;
    // }
    // else if (strcmp(command, "startFish") == 0) {
    //     char *arg = strtok(NULL, "");
    //     printf("startFish %s\n", arg);
    //     return;
    // }
    // else if (strcmp(command, "hello") == 0) {
    //     command = strtok(NULL, " ");
    //     if (command == NULL) {
    //         printf("greetings xx\n");
    //     }
    //     if (strcmp(command, "in") != 0) {
    //         printf("Command should be hello in as <ID>\n");
    //         exit(0);
    //     }
    //     command = strtok(NULL, " ");
    //     if (strcmp(command, "as") != 0) {
    //         printf("Command should be hello in as <ID>\n");
    //         exit(0);
    //     }
    //     char *ID = strtok(NULL, " ");
    //     printf("greeting %s\n", ID);
    //     return;
    // }
    // else if (strcmp(command, "getFishes") == 0) {
    //     printf("printing periodically a list of fishes\n");
    //     return;
    // }
    // else if (strcmp(command, "ls") == 0 || strcmp(command, "getFishesContinuously") == 0) {
    //     printf("printing continuously a list of fishes\n");
    //     return;
    // }
    // else if (strcmp(command, "ping") == 0) {
    //     char *arg = strtok(NULL, " ");
    //     printf("pong %d\n", atoi(arg));
    //     return;
    // } 
    else
        printf("unknown command\n");
    free_parser(p);
    return NULL;
}

int main(int argc, char * argv[]){
    struct parse *result = parser("addFish PoissonNain at 61x52, 4x3, RandomWayPoint");
    if (result !=NULL){
        printf("%d\n", result->size);
        for (int i =0; i<result->size; i++){
            printf("%s\n", result->tab[i]);
        }
    }
    else printf("%p\n", result);
    return 0;
}
