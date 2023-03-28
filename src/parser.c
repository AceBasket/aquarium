#include <string.h>
#include<stdio.h>
#include <stdlib.h>


void  parser(char * str){
    char * command;
    command = strtok(str, " ");
    if (strcpy(command, "load")==0){
        char *arg = strtok(NULL, " ");
        printf("load %s\n", arg);
    }
    if (strcpy(command, "show")==0){    
        char * arg = strtok(NULL, " ");
        printf("show %s\n", arg);
    }
    if (strcpy(command, "add") == 0){
        command = strtok(NULL, " ");
        if(strcpy(command, "view") !=0 ){
            printf("Wrong command\n");
            exit(0);
        }
        char *arg1 = strtok(NULL, " ");
        char *arg2 = strtok(NULL, "x");
        if(arg2==NULL){
            printf("Wrong command\n");
            exit(0);
        }
        char *arg3 = strtok(NULL, "+");
        char *arg4 = strtok(NULL, "+");
        char *arg5 = strtok(NULL, "");
        printf("add %s %s %s %s %s\n", arg1, arg2, arg3, arg4, arg5);
        //add_view(arg1, arg2, atoi(arg3), atoi(arg4), atoi(arg5), )
    }
    if (strcpy(command, "del")==0){
        command = strtok(NULL, " ");
        if(strcpy(command, "view")!= 0){
            printf("Wrong command\n");
            exit(0);
        }
        char * arg = strtok(str, " ");
        printf("del %s\n", arg);
    }
    if (strcpy(command, "save")==0){
        char * arg = strtok(NULL, " ");
        printf("save %s\n", arg);
    }
    if (strcpy(command, "status")==0){
       printf("status");
    }
    if (strcpy(command, "addFish")==0){
        char * arg1 = strtok(NULL, " ");
        command = strtok(NULL, " ");
        if(strcpy(command, "at") != 0){
            printf("Wrong command\n");
            exit(0);
        }
        char *arg2 = strtok(NULL, "x");
        char *arg3 = strtok(NULL, ", ");
        char *arg4 = strtok(NULL, "x");
        char *arg5 = strtok(NULL, ", ");
        char *arg6 = strtok(NULL, "");
        printf("addFish %s %s %dx%d, %dx%d, %s\n", arg1, command, atoi(arg2), atoi(arg3), atoi(arg4), atoi(arg5), arg6 );
    }
    if (strcpy(command, "delFish")==0){
        char * arg = strtok(NULL, "");
        printf("delFish %s\n", arg);
    }
    if (strcpy(command, "startFish")==0){
        char * arg = strtok(NULL, "");
        printf("startFish %s\n", arg);
    }
    if (strcpy(command, "hello")==0){
        command = strtok(NULL, " ");
        if (command ==NULL){
            printf("greetings xx\n");
        }
        if(strcpy(command, "in") != 0){
            printf("Command should be hello in as <ID>\n");
            exit(0);
        } 
        command = strtok(NULL, " ");
        if(strcpy(command, "as") != 0){
            printf("Command should be hello in as <ID>\n");
            exit(0);
        } 
        char * ID = strtok(NULL, " ");
        printf("greeting %s\n", ID);
    } 
    if (strcpy(command, "getFishes")==0){
        printf("printing periodically a list of fishes\n");
    }
    if (strcpy(command, "ls")==0 || strcpy(command, "getFishesContinuously")==0){
            printf("printing continuously a list of fishes\n");
    }
    if (strcpy(command, "ping")==0){
        char * arg = strtok(NULL, " ");
        printf("pong %d\n", atoi(arg));
    }
    else
        printf("uknown command");
    
}