/*
** EPITECH PROJECT, 2023
** testC
** File description:
** logger.h
*/

#pragma once

#define _GNU_SOURCE
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <time.h>

/**
 * @brief Maximum number of loggers allowed.
 */
#define MAX_LOGGERS_NBR 8

/**
 * @brief Macro to create an empty stream (e.g., /dev/null).
 *
 * This macro can be used to initialize a stream to /dev/null for cases where
 * no logging is desired.
 */
#define EMPTY_STREAM fopen("/dev/null", "w")

#ifdef __linux__
#include <unistd.h>

/**
 * @brief Macro to open a log file stream with the binary name concatenated with ".log".
 *
 * This macro retrieves the name of the binary file being executed by reading the
 * symbolic link "/proc/self/exe" and concatenates ".log" to it. It then opens
 * a writable binary file stream.
 */
#define BINARY_FILENAME_LOG_STREAM fopen(({                             \
    static char log_file[PATH_MAX + 4];                             \
    ssize_t len = readlink("/proc/self/exe", log_file, PATH_MAX);   \
    if (len != -1 && len <= PATH_MAX - 4) {                         \
        log_file[len] = '\0';                                       \
        snprintf(log_file + len, sizeof(log_file) - len, ".log");   \
    } else {                                                        \
        log_file[0] = '\0';                                         \
    }                                                               \
    log_file;                                                       \
}), "a")

#elif __APPLE__
#include <mach-o/dyld.h>

/**
 * @brief Macro to open a log file stream with the binary name concatenated with ".log".
 *
 * This macro retrieves the name of the binary file being executed using the
 * _NSGetExecutablePath function and concatenates ".log" to it. It then opens
 * a writable binary file stream.
 */
#define BINARY_FILENAME_LOG_STREAM fopen(({                             \
    static char log_file[PATH_MAX + 4];                             \
    uint32_t size = sizeof(log_file) - 4;                           \
    if (_NSGetExecutablePath(log_file, &size) == 0) {               \
        size_t len = strlen(log_file);                              \
        snprintf(log_file + len, sizeof(log_file) - len, ".log");   \
    } else {                                                        \
        log_file[0] = '\0';                                         \
    }                                                               \
    log_file;                                                       \
}), "a")

#elif _WIN32
#include <windows.h>

/**
 * @brief Macro to open a log file stream with the binary name concatenated with ".log".
 *
 * This macro retrieves the name of the binary file being executed using the
 * GetModuleFileName function and concatenates ".log" to it. It then opens
 * a writable binary file stream.
 */
#define BINARY_FILENAME_LOG_STREAM fopen(({                             \
    static char log_file[MAX_PATH + 4];                             \
    DWORD len = GetModuleFileName(NULL, log_file, MAX_PATH);        \
    if (len > 0 && len <= MAX_PATH - 4) {                           \
        snprintf(log_file + len, sizeof(log_file) - len, ".log");   \
    } else {                                                        \
        log_file[0] = '\0';                                         \
    }                                                               \
    log_file;                                                       \
}), "a")

#endif

/**
 * @brief Macro to check if the stream in a logger configuration is a standard stream (stdout or stderr).
 *
 * @param c Pointer to the log_config_t structure.
 * @return true if the stream is stdout or stderr, false otherwise.
 */
#define IS_STD_STREAM(c) (c->stream == stdout || c->stream == stderr)

/**
 * @brief Macro to create a log style with a specified color and style.
 *
 * @param color The color of the log text (e.g., RED, GREEN).
 * @param style The style of the log text (e.g., BOLD, UNDERLINE).
 */
#define LOG_STYLE(color, style) (log_style_t) {color, style}

/**
 * @brief Macro to convert a value to a string literal.
 *
 * @param x The value to be converted to a string.
 */
#define __STRINGIFY(x) #x

/**
 * @brief Macro to convert a macro value to a string literal.
 *
 * @param x The macro value to be converted to a string.
 */
#define TOSTRING(x) __STRINGIFY(x)

/**
 * @brief Macro to format the file and line information.
 */
#define _FL "[" __FILE__ ":" TOSTRING(__LINE__) "]"

/**
 * @brief Macro for a basic log message with file and line information.
 *
 * @param message The message to be logged.
 */
#define BASIC_LOG(message) log_message(message, _FL)

/**
 * @brief Macro for a formatted log message with file and line information.
 *
 * This macro logs a formatted message along with the file name and line number
 * from where the log is invoked. It accepts a format string and additional
 * arguments similar to printf.
 *
 * @param format The format string.
 * @param ... Additional arguments for the format string.
 */
#define BASIC_LOG_F(format, ...) log_message_formated(_FL, format, __VA_ARGS__)

/**
 * @brief Macro to log a message with a specified logger name and file/line info.
 *
 * @param message The message to be logged.
 * @param logger_name The name of the logger to use.
 */
#define DLOG(message, logger_name)  log_message(_FL " => " message, logger_name)
#define LOG(message, logger_name)   log_message(message, logger_name)

/**
 * @brief Macro to log a formatted message with a specified logger name and file/line info.
 *
 * This macro logs a formatted message with a specified logger name, including
 * the file name and line number from where the log is invoked. It accepts a
 * format string and additional arguments similar to printf.
 *
 * @param logger_name The name of the logger to use.
 * @param format The format string.
 * @param ... Additional arguments for the format string.
 */
#define DLOG_F(logger_name, format, ...)    log_message_formated(logger_name, _FL " => " format, __VA_ARGS__)
#define LOG_F(logger_name, format, ...)     log_message_formated(logger_name, format, __VA_ARGS__)

/**
 * @brief Macro to generate a string with the current date and time.
 *
 * This macro generates a string with the current date, hour, minutes, and seconds
 * in the format: "YYYY-MM-DD HH:MM:SS".
 */
#define CURRENT_DATETIME_STRING ({                                                  \
    time_t t = time(NULL);                                                          \
    struct tm *tm_info = localtime(&t);                                             \
    static char datetime_str[20];                                                   \
    strftime(datetime_str, sizeof(datetime_str), "%Y-%m-%d-%H:%M:%S", tm_info);     \
    datetime_str;                                                                   \
})

/**
 * @brief Enum for log colors.
 */
enum log_color_e {
    RED = 31,      ///< Red color
    GREEN = 32,    ///< Green color
    YELLOW = 33,   ///< Yellow color
    BLUE = 34,     ///< Blue color
    MAGENTA = 35,  ///< Magenta color
    CYAN = 36,     ///< Cyan color
    COLOR_NONE = 0 ///< No color
};

/**
 * @brief Enum for log styles.
 */
enum log_style_e {
    STYLE_NONE = 0,  ///< No style
    BOLD = 1,        ///< Bold style
    UNDERLINE = 4,   ///< Underline style
    BLINK = 5,       ///< Blink style
    INVERSE = 7      ///< Inverse style
};

/**
 * @brief Structure for log style, combining color and style.
 */
typedef struct log_style_s {
    enum log_color_e color;  ///< Log color
    enum log_style_e style;  ///< Log style
} log_style_t;

/**
 * @brief Structure for logger configuration.
 */
typedef struct log_config_s {
    FILE *stream;       ///< Output stream for the logger
    char *name;         ///< Name of the logger
    log_style_t style;  ///< Style of the logger
} log_config_t;

/**
 * @brief Initialize basic loggers. This function is called automatically.
 */
void init_basics_loggers(void) __attribute__((constructor(101)));

/**
 * @brief Log a message with a specified logger name.
 *
 * @param message The message to be logged.
 * @param logger_name The name of the logger to use.
 */
void log_message(const char *, const char *);

/**
 * @brief Log a formatted message with a specified logger name.
 *
 * This function formats a message according to the provided format string
 * and additional arguments, and then logs it using the specified logger.
 *
 * @param logger_name The name of the logger to use.
 * @param format The format string (as in printf).
 * @param ... Additional arguments for the format string.
 */
void log_message_formated(const char *logger_name, const char *format, ...);

/**
 * @brief Destroy all loggers. This function is called automatically.
 */
void destroy_loggers(void) __attribute__((destructor));

/**
 * @brief Get logger configuration by name.
 *
 * @param name The name of the logger to retrieve.
 * @return A pointer to the logger configuration, or NULL if not found.
 */
log_config_t *get_logger_by_name(const char *);

/**
 * @brief Register a new logger with a name, output stream, and style.
 *
 * @param name The name of the logger to register.
 * @param stream The output stream for the logger.
 * @param style The style for the logger.
 */
void register_logger(const char *, FILE *, log_style_t);

/**
 * @brief Set the output stream for a specified logger.
 *
 * @param logger_name The name of the logger to configure.
 * @param stream The new output stream for the logger.
 */
static inline void set_logger_stream(const char *logger_name, FILE *stream)
{
    log_config_t *logger = get_logger_by_name(logger_name);

    if (!logger) {
        log_message("Logger not found", "WARNING");
        return;
    }
    logger->stream = stream;
}

/**
 * @brief Set the name for a specified logger.
 *
 * @param logger_name The name of the logger to configure.
 * @param name The new name for the logger.
 */
static inline void set_logger_name(const char *logger_name, const char *name)
{
    log_config_t *logger = get_logger_by_name(logger_name);

    if (!logger) {
        log_message("Logger not found", "WARNING");
        return;
    }
    free(logger->name);
    logger->name = strdup(name);
}

/**
 * @brief Set the style for a specified logger.
 *
 * @param logger_name The name of the logger to configure.
 * @param style The new style for the logger.
 */
static inline void set_logger_style(const char *logger_name, log_style_t style)
{
    log_config_t *logger = get_logger_by_name(logger_name);

    if (!logger) {
        log_message("Logger not found", "WARNING");
        return;
    }
    logger->style = style;
}