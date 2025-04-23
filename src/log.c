#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <pthread.h>


static void print_with_color(const char *prefix, const char *color_code, const char *format, va_list args) {
    pthread_mutex_lock(&log_mutex);

    printf("%s%s", color_code, prefix);
    vprintf(format, args);
    printf("\033[0m");

    fflush(stdout); // Garante que a saída seja imediata

    pthread_mutex_unlock(&log_mutex);
}

void print_red(const char *format, ...) {
    va_list args;
    va_start(args, format);
    print_with_color("[Santa]: ", "\033[0;31m", format, args);
    va_end(args);
}

void print_green(const char *format, ...) {
    va_list args;
    va_start(args, format);
    print_with_color("[Elfo]: ", "\033[0;32m", format, args);
    va_end(args);
}

void print_yellow(const char *format, ...) {
    va_list args;
    va_start(args, format);
    print_with_color("[Rena]: ", "\033[0;33m", format, args);
    va_end(args);
}

void print_blue(const char *format, ...) {
    va_list args;
    va_start(args, format);
    print_with_color("[System]: ", "\033[0;34m", format, args);
    va_end(args);
}
