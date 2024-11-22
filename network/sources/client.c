/*
** EPITECH PROJECT, 2023
** zappy_server
** File description:
** client.c
*/

#include "client.h"
#include "server.h"
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "logger.h"

static void create_client(server_t *server, fd_t fd, struct sockaddr_in *addr)
{
    char client_ip[INET_ADDRSTRLEN];
    client_t *client = calloc(1, sizeof(client_t));

    if (!client) {
        DLOG_F("ERROR", "%s\n", strerror(errno));
        return;
    }
    memcpy(&client->infos.addr, addr, sizeof(struct sockaddr_in));
    client->infos.time = time(NULL);
    client->socket = fd;
    init_queue(&client->in_msg);
    init_queue(&client->out_msg);
    STAILQ_INIT(&client->buffer);
    inet_ntop(AF_INET, &(client->infos.addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    LOG_F("NETWORK", "Connection accepted from %s:%d", client_ip, ntohs(addr->sin_port));
    server->new_client = client;
    STAILQ_INSERT_HEAD(&server->clients, client, __entries);
}

void client_stringify(client_t *client, char *buffer)
{
    inet_ntop(AF_INET, &(client->infos.addr.sin_addr), buffer, INET_ADDRSTRLEN);
}

int accept_client(server_t *server)
{
    int new = accept(server->socket, (struct sockaddr *)&server->addr, &server->socket_len);

    if (new == -1)
        return FAILED_TO_ACCEPT_CLIENT;
    if (server->nb_clients >= MAX_CLIENTS) {
        close(new);
        return NO_ERR;
    }
    create_client(server, new, &server->addr);
    server->nb_clients++;
    return NO_ERR;
}

char *extract_buffer(struct buffer_queue *buffer, char *message_end, uint16_t end_size)
{
    buffer_t *elem = NULL;
    char *message = NULL;
    size_t message_size = end_size;
    size_t ptr = 0;

    STAILQ_FOREACH(elem, buffer, entries) {
        message_size += elem->wh;
    }
    if (!(message = calloc(message_size + 1, 1))) {
        perror("buffer");
        return NULL;
    }
    while ((elem = dequeue_buffer(buffer))) {
        for (uint16_t i = 0; elem->text[i]; i++, ptr++)
            message[ptr] = elem->text[i];
        free(elem);
    }
    STAILQ_INIT(buffer);
    memcpy(message + ptr, message_end, end_size);
    return message;
}

int disconnect_client(server_t *server, client_t *client)
{
    buffer_t *buff = NULL;
    msg_t *msg = NULL;

    while ((buff = dequeue_buffer(&client->buffer))) {
        free(buff);
    }
    while ((msg = dequeue_message(&client->out_msg))) {
        free_message(msg);
    }
    while ((msg = dequeue_message(&client->in_msg))) {
        free_message(msg);
    }
    if (client->socket != INVALID_SOCKET) {
        close(client->socket);
        client->socket = INVALID_SOCKET;
        server->nb_clients--;
    }
    LOG("DISCONNECT", "NETWORK");
    return NO_ERR;
}

static ssize_t make_buffer(char *buffer, struct message_queue *q)
{
    ssize_t i = 0;
    msg_t *msg = NULL;

    memset(buffer, 0, WRITE_SIZE);
    while ((msg = STAILQ_FIRST(q))) {
        if (msg->size - msg->wh < (size_t)(WRITE_SIZE - i)) {
            memcpy(buffer + i, msg->text + msg->wh, msg->size - msg->wh);
            i += msg->size - msg->wh;
            memcpy(buffer + i, "\n", 1);
            i++;
            STAILQ_REMOVE_HEAD(q, entries);
            free_message(msg);
        } else {
            memcpy(buffer + i, msg->text, WRITE_SIZE - i);
            msg->wh += WRITE_SIZE - i;
            return WRITE_SIZE;
        }
    }
    return i;
}

int client_write(client_t *client)
{
    msg_t *msg = STAILQ_FIRST(&client->out_msg);
    ssize_t n_bytes = 0;
    char tmp[WRITE_SIZE];

    if (!msg)
        return NO_ERR;
    if (msg->size - msg->wh < WRITE_SIZE) {
        n_bytes = make_buffer(tmp, &client->out_msg);
        if (n_bytes > 0)
            n_bytes = write(client->socket, tmp, n_bytes);
    } else {
        n_bytes = write(client->socket, msg->text + msg->wh, WRITE_SIZE);
        if (n_bytes > 0)
            msg->wh += n_bytes;
    }
    if (!n_bytes)
        return CLIENT_DISCONNECT;
    if (n_bytes == -1)
        return FAILED_TO_SEND_DATA;
    return NO_ERR;
}

int client_read(client_t *client)
{
    char tmp[READ_SIZE];
    ssize_t n_bytes = 0;
    char *message = NULL;

    memset(&tmp, 0, sizeof(tmp));
    n_bytes = read(client->socket, &tmp, sizeof(tmp));
    if (n_bytes == -1)
        return FAILED_TO_READ_DATA;
    if (!n_bytes)
        return CLIENT_DISCONNECT;
    for (uint16_t i = 0; i < n_bytes; i++) {
        if (tmp[i] != '\n')
            continue;
        message = extract_buffer(&client->buffer, tmp, i);
        if (!message)
            return FAILED_TO_READ_DATA;
        enqueue_message(&client->in_msg, message);
        free(message);
        if (i != n_bytes - 1)
            enqueue_buffer(&client->buffer, tmp + i - 1, n_bytes - i);
        return NO_ERR;
    }
    enqueue_buffer(&client->buffer, tmp,  n_bytes);
    return NO_ERR;
}
