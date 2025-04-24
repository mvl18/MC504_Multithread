#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>

static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

// Criação de buffer circular para armazenar as mensagens de log e adicionar no SDL
char log_buffer[LOG_LINES][LOG_LINE_LENGTH];
int log_index = 0;

void add_to_log(const char *message) {
    pthread_mutex_lock(&log_mutex);
    snprintf(log_buffer[log_index], LOG_LINE_LENGTH, "%s", message);
    log_index = (log_index + 1) % LOG_LINES; // Atualiza o índice circularmente
    pthread_mutex_unlock(&log_mutex);
}

// Remove sequências ANSI do texto
void remove_ansi_sequences(const char *input, char *output) {
    const char *src = input;
    char *dst = output;

    while (*src) {
        if (*src == '\033') { // Detecta início de sequência ANSI
            while (*src && *src != 'm') src++; // Ignora até o final da sequência
            if (*src) src++; // Ignora o 'm'
        } else {
            *dst++ = *src++; // Copia caracteres normais
        }
    }
    *dst = '\0'; // Finaliza a string
}

void remove_newline(char *text) {
    char *newline = strchr(text, '\n'); // Procura o caractere '\n'
    if (newline) {
        *newline = '\0'; // Substitui '\n' por '\0' (fim de string)
    }
}

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

    // Formata a mensagem e adiciona ao buffer circular
    char message[LOG_LINE_LENGTH];
    vsnprintf(message, LOG_LINE_LENGTH, format, args);
    add_to_log(message);

    print_with_color("[Santa]: ", "\033[0;31m", format, args);
    va_end(args);
}

void print_green(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char message[LOG_LINE_LENGTH];
    vsnprintf(message, LOG_LINE_LENGTH, format, args);
    add_to_log(message);

    print_with_color("[Elfo]: ", "\033[0;32m", format, args);
    va_end(args);
}

void print_yellow(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char message[LOG_LINE_LENGTH];
    vsnprintf(message, LOG_LINE_LENGTH, format, args);
    add_to_log(message);

    print_with_color("[Rena]: ", "\033[0;33m", format, args);
    va_end(args);
}

void print_blue(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char message[LOG_LINE_LENGTH];
    vsnprintf(message, LOG_LINE_LENGTH, format, args);
    add_to_log(message);

    print_with_color("[System]: ", "\033[0;34m", format, args);
    va_end(args);
}
