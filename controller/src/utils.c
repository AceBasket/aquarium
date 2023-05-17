#include "utils.h"
#include <stdlib.h>

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

void sigpipe_handler(int signum) {
    (void)signum;
    fprintf(stdout, "SIGPIPE received\n");
    terminate_threads = OK;
    exit(EXIT_FAILURE);
}


