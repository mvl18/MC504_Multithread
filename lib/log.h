#ifndef __LOG_H__
#define __LOG_H__

#define LOG_LINES 9
#define LOG_LINE_LENGTH 256

extern char log_buffer[LOG_LINES][LOG_LINE_LENGTH];
extern int log_index;

void add_to_log(const char *message);

const char *get_from_log(int i);

void remove_ansi_sequences(const char *input, char *output);

void remove_newline(char *text);

/**
 * @brief Print message in red color
 * 
 * @param format 
 * @param ... 
 * @return void 
 */
void print_red(const char *format, ...);

/**
 * @brief Print message in green color
 * 
 * @param format 
 * @param ... 
 * @return void 
 */
void print_green(const char *format, ...);

/**
 * @brief Print message in yellow color
 * 
 * @param format 
 * @param ... 
 * @return void 
 */
void print_yellow(const char *format, ...);

/**
 * @brief Print debug message in blue color
 * 
 * @param format 
 * @param ... 
 * @return void 
 */
void print_blue(const char *format, ...);

#endif // __LOG_H__