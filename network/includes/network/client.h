/*
** EPITECH PROJECT, 2023
** zappy_server
** File description:
** client.h
*/

#pragma once

#include "network.h"
#include <time.h>
#include <sys/types.h>

#define BUFFER_SIZE 4096
#define READ_SIZE 1024
#define WRITE_SIZE READ_SIZE

_Static_assert(READ_SIZE <= BUFFER_SIZE, \
    "READ_SIZE must be less than or equal to BUFFER_SIZE");

typedef struct buffer {
    char text[4096];
    uint16_t wh;
    STAILQ_ENTRY(buffer) entries;
} buffer_t;

STAILQ_HEAD(buffer_queue, buffer);

typedef struct client_s {
    fd_t socket;
    msgq_t in_msg;
    msgq_t out_msg;
    struct {
        struct sockaddr_in addr;
        time_t time;
    } infos;
    struct buffer_queue buffer;
    STAILQ_ENTRY(client_s) __entries;
} client_t;

STAILQ_HEAD(client_queue, client_s);

typedef struct client_queue clientq_t;

#define IS_DISCONNECT(client) client->socket == INVALID_SOCKET

int client_read(client_t *client);
int enqueue_buffer(struct buffer_queue *q, const char *text, ssize_t text_len);
struct buffer *dequeue_buffer(struct buffer_queue *q);
int client_write(client_t *client);
void client_stringify(client_t *client, char *buffer);
char *extract_buffer(struct buffer_queue *buffer, char *message_end, uint16_t end_size);

#define CLIENT_SEND(client, message) do {enqueue_message(&client->out_msg, message); FD} while (0);
