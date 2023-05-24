#include "utils.h"
#include <sys/time.h>
#include <math.h>

#ifdef TESTS
enum log_level verbosity_level = LOG_INFO;
#endif

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

unsigned long long get_time_in_milliseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000;
}

unsigned long long add_seconds_to_time_in_milliseconds(unsigned long long time, int seconds) {
    return time + (unsigned long long)seconds * 1000;
}

time_t get_seconds_to_get_to_time_in_milliseconds(unsigned long long time) {
    unsigned long long current_time = get_time_in_milliseconds();
    if (current_time > time) {
        return 0;
    }
    return (time_t)round((time - current_time) / 1000);
}

void log_message(FILE *log, enum log_level level, const char *message, ...) {
    va_list args;
    va_start(args, message);
    // format the message into a temporary buffer (with the interpretation of %...)
    char buffer[256] = {};
    vsnprintf(buffer, sizeof(buffer), message, args);

    // fog informational messages
    if (level == LOG_INFO && verbosity_level >= LOG_INFO) {
        fprintf(log, "%s\n", buffer);

    // log warning messages
    } else if (level == LOG_WARNING && verbosity_level >= LOG_WARNING) {
        fprintf(log, "WARNING: %s\n", buffer);

    // log error messages
    } else if (level == LOG_ERROR && verbosity_level >= LOG_ERROR) {
        if (errno != 0) {
            fprintf(log, "ERROR: %s -> %s\n", buffer, strerror(errno));
        } else {
            fprintf(log, "ERROR: %s\n", buffer);
        }

    // log fatal error messages and exit the program
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

#ifndef TESTS
void sigpipe_handler(int signum) {
    (void)signum;
    fprintf(stdout, "SIGPIPE received\n");
    terminate_threads = OK;
    exit(EXIT_FAILURE);
}
#endif

