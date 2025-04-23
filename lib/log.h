#ifndef __LOG_H__
#define __LOG_H__

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