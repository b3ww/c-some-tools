/*
** EPITECH PROJECT, 2023
** zappy_server
** File description:
** message_queue.c
*/

#include "message_queue.h"
#include <string.h>
#include <stdlib.h>
#include "logger.h"

void init_queue(struct message_queue *q)
{
    STAILQ_INIT(q);
}

int enqueue_message(struct message_queue *q, const char *text)
{
    struct message *msg = calloc(1, sizeof(struct message));

    if (!msg) {
        DLOG("Failed to allocate message", "ERROR");
        return 1;
    }
    msg->text = strdup(text);
    msg->size = strlen(text);
    msg->wh = 0;
    if (!msg->text) {
        DLOG("Failed to duplicate message text", "ERROR");
        free(msg);
        return 1;
    }
    STAILQ_INSERT_TAIL(q, msg, entries);
    return 0;
}

struct message *dequeue_message(struct message_queue *q)
{
    struct message *msg = STAILQ_FIRST(q);

    if (msg)
        STAILQ_REMOVE_HEAD(q, entries);
    return msg;
}

void free_message(struct message *msg)
{
    if (msg) {
        free(msg->text);
        free(msg);
    }
}

size_t queue_size(struct message_queue *q)
{
    size_t count = 0;
    struct message *msg;

    STAILQ_FOREACH(msg, q, entries) {
        count++;
    }
    return count;
}
