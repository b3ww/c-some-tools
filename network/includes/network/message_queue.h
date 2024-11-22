/*
** EPITECH PROJECT, 2023
** zappy_server
** File description:
** message_queue.h
*/

#pragma once

#include <sys/queue.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>

struct message {
    char *text;
    size_t size;
    ssize_t wh;
    STAILQ_ENTRY(message) entries;
};

STAILQ_HEAD(message_queue, message);

void init_queue(struct message_queue *q);
int enqueue_message(struct message_queue *q, const char *text);
struct message *dequeue_message(struct message_queue *q);
void free_message(struct message *msg);
size_t queue_size(struct message_queue *q);

static inline bool have_message(struct message_queue *q)
{
    return (bool)STAILQ_FIRST(q);
}
