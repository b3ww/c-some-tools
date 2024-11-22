/*
** EPITECH PROJECT, 2023
** zappy_server
** File description:
** buffer_queue.c
*/

#include "client.h"
#include <string.h>
#include <stdlib.h>
#include "logger.h"

int enqueue_buffer(struct buffer_queue *q, const char *text, ssize_t text_len)
{
    struct buffer *msg = NULL;

    STAILQ_FOREACH(msg, q, entries);
    if (msg && ((size_t)text_len <= sizeof(msg->text) - msg->wh)) {
        memcpy(msg->text + msg->wh, text, text_len);
        msg->wh += text_len;
        return 0;
    }
    msg = calloc(1, sizeof(struct buffer));
    if (!msg) {
        DLOG("Failed to allocate message", "ERROR");
        return 1;
    }
    memcpy(msg->text, text, text_len);
    msg->wh = text_len;
    STAILQ_INSERT_TAIL(q, msg, entries);
    return 0;
}

struct buffer *dequeue_buffer(struct buffer_queue *q)
{
    struct buffer *msg = STAILQ_FIRST(q);

    if (msg)
        STAILQ_REMOVE_HEAD(q, entries);
    return msg;
}
