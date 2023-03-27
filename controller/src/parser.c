#include <string.h>
#include<stdio.h>
#include <stdlib.h>


void  parser(char * str){
    char * command;
    command = strtok(str, " ");
    switch (command){
        case "load":
            char *arg = strtok(NULL, " ");
            //load(arg);
            printf("load %s\n", arg);
            break;
        case "show":
            char * arg = strtok(NULL, " ");
            printf("show %s\n", arg);
            //show(arg);
            break;
        case "add":
            command = strtok(str, " ");
            if(strcpy(command, "view")< 0){
                print("Wrong command\n");
                exit(0);
            }
            char *arg1 = strtok(NULL, " ");
            char *arg2 = strtok(NULL, "x");
            char *arg3 = strtok(NULL, "+");
            char *arg4 = strtok(NULL, "+");
            char *arg5 = strtok(NULL, "");
            printf("add %s %s %s %s %s\n", arg1, arg2, arg3, arg4, arg5);
            //add_view(arg1, arg2, atoi(arg3), atoi(arg4), atoi(arg5), )
            break;
        case "del":
            command = strtok(str, " ");
            if(strcpy(command, "view")< 0){
                print("Wrong command\n");
                exit(0);
            }
            char * arg = strtok(str, " ");
            printf("del %s\n", arg);
            //del_view(arg);
            break;
        case "save":
            char * arg = strtok(NULL, " ");
            printf("save %s\n", arg);
            //save(arg);
            break;
        default:
            printf("uknown command");
    }
}