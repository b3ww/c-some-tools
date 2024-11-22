/*
** EPITECH PROJECT, 2023
** testC
** File description:
** logger.c
*/

#include "logger.h"

static log_config_t *get_logger(void)
{
    static log_config_t loggers[MAX_LOGGERS_NBR];

    return loggers;
}

static void put_log_message(log_config_t *config, const char *message)
{
    if (config->style.color != COLOR_NONE)
        fprintf(config->stream, "\x1b[%dm", config->style.color);
    if (config->style.style != STYLE_NONE)
        fprintf(config->stream, "\x1b[%dm", config->style.style);
    fprintf(config->stream, "%s => %s", config->name, message);
    if (config->style.color != COLOR_NONE)
        fprintf(config->stream, "\x1b[0m");
    switch (config->style.style) {
        case BOLD:
            fprintf(config->stream, "\x1b[22m");
        case UNDERLINE:
            fprintf(config->stream, "\x1b[24m");
        case BLINK:
            fprintf(config->stream, "\x1b[25m");
        case INVERSE:
            fprintf(config->stream, "\x1b[27m");
        default:
            break;
    }
    fprintf(config->stream, "\n");
}

void init_basics_loggers(void)
{
    log_config_t *loggers = get_logger();

    ((log_config_t *)(loggers + 0))->stream = stderr;
    ((log_config_t *)(loggers + 0))->name = strdup("ERROR");
    ((log_config_t *)(loggers + 0))->style = (log_style_t){RED, BOLD};
    ((log_config_t *)(loggers + 1))->stream = stderr;
    ((log_config_t *)(loggers + 1))->name = strdup("WARNING");
    ((log_config_t *)(loggers + 1))->style = (log_style_t){MAGENTA, BOLD};
    ((log_config_t *)(loggers + 2))->stream = stdout;
    ((log_config_t *)(loggers + 2))->name = strdup("INFO");
    ((log_config_t *)(loggers + 2))->style = (log_style_t){GREEN, BOLD};
    for (int i = 3; i < MAX_LOGGERS_NBR; ++i)
        ((log_config_t *)(loggers + i))->stream = NULL;
}

void log_message(const char *message, const char *logger_name)
{
    log_config_t *loggers = get_logger();
    log_config_t *tmp = NULL;

    for (int i = 0; i < MAX_LOGGERS_NBR; i++) {
        tmp = ((log_config_t *)(loggers + i));
        if (tmp->stream && !strcmp(tmp->name, logger_name))
            break;
    }
    if (!tmp->stream) {
        printf("%s => %s\n", logger_name, message);
        return;
    }
    if (!IS_STD_STREAM(tmp)) {
        fseek(tmp->stream, 0, SEEK_END);
        fprintf(tmp->stream, "%s | %s => %s\n", CURRENT_DATETIME_STRING, logger_name, message);
        return;
    }
    put_log_message(tmp, message);
}

log_config_t *get_logger_by_name(const char *name)
{
    log_config_t *loggers = get_logger();
    log_config_t *tmp = NULL;

    for (int i = 0; i < MAX_LOGGERS_NBR; i++) {
        tmp = ((log_config_t *)(loggers + i));
        if (tmp->stream && !strcmp(tmp->name, name))
            return tmp;
    }
    return NULL;
}

void destroy_loggers(void)
{
    log_config_t *loggers = get_logger();
    log_config_t *tmp = NULL;

    for (int i = 0; i < MAX_LOGGERS_NBR; i++) {
        tmp = ((log_config_t *)(loggers + i));
        if (tmp->stream) {
            free(tmp->name);
            // fclose(tmp->stream);
        }
    }
}

void register_logger(const char *name, FILE *stream, log_style_t style)
{
    log_config_t *loggers = get_logger();
    log_config_t *tmp = NULL;

    for (int i = 0; i < MAX_LOGGERS_NBR; i++) {
        tmp = ((log_config_t *)(loggers + i));
        if (!tmp->stream) {
            tmp->name = strdup(name);
            tmp->stream = stream;
            tmp->style = style;
            return;
        }
    }
    log_message("Logger max nb was exceded", "WARNING");
}

void log_message_formated(const char *logger_name, const char *format, ...)
{
    char *str = NULL;
    va_list args;

    va_start(args, format);
    vasprintf(&str, format, args);
    va_end(args);
    log_message(str, logger_name);
    free(str);
}
