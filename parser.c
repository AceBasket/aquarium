#include <string.h>
#include<stdio.h>
#include <stdlib.h>


(void*)(*f)(void*) parser(char * str){
    char * command;
    command = strtok(str, " ");
    switch (command){
        case "load":
            char *arg = strtok(NULL, " ");
            return load(arg);
            break;
        case "show":
            char * arg = strtok(NULL, " ");
            return show(arg);
            break;
        case "add":
            command = strtok(str, " ");
            if(strcpy(command, "view")< 0){
                print("Wrong command\n");
                exit(0);
            }
            char *arg1 = strtok(NULL, " ");
            char *arg2 = 
            return (pch);
            break;
        case "del":
            break;
        case "save":
            break;
        default:
            printf("uknown command");
    }
}