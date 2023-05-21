#include "utils.h"

void exit_if(int condition, const char *message) {
    if (condition) {
        if (errno != 0) {
            perror(message);
        } else {
            fprintf(stderr, "%s\n", message);
        }
        exit(1);
    }
}


void log_message(FILE *log, enum log_level level, const char* message, ...) {
    va_list args;
    va_start(args, message);
    // Format the message into a temporary buffer (with the interpretation of %...)
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), message, args);
    
    // Log informational messages
    if (level == LOG_INFO && verbosity_level >= LOG_INFO) {
        fprintf(log, "%s\n", buffer);

    // Log warning messages
    } else if (level == LOG_WARNING && verbosity_level >= LOG_WARNING) {
        fprintf(log, "WARNING: %s\n", buffer);    

    // Log error messages
    } else if (level == LOG_ERROR && verbosity_level >= LOG_ERROR) {
        if (errno != 0) {
            fprintf(log, "ERROR: %s -> %s\n", buffer, strerror(errno));
        } else {
            fprintf(log, "ERROR: %s\n", buffer);
        }

    // Log fatal error messages and exit the program
    } else if (level == LOG_FATAL_ERROR) {
        if (verbosity_level >= LOG_FATAL_ERROR) {
            if (errno != 0) {
                fprintf(log, "FATAL ERROR: %s -> %s\n", buffer, strerror(errno));
            } else {
                fprintf(log, "FATAL ERROR: %s\n", buffer);
            }
        }
        exit(1);
    }
    fflush(log);
    va_end(args);
}


void sigpipe_handler(int signum) {
    (void) signum;
    fprintf(stdout, "SIGPIPE received\n");
    terminate_threads = OK;
    exit(EXIT_FAILURE);
}

