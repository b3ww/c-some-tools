/*
** EPITECH PROJECT, 2023
** zappy_server
** File description:
** server.h
*/

#pragma once

#include "network.h"
#include "client.h"
#include <sys/time.h>

#define MAX_CLIENTS 1000

#define GET_CLIENT(serv, index) serv->clients[index]

#define FOREACH_VALID_CLIENT(serv, curr, expr)          \
    client_t *curr = NULL;                              \
    STAILQ_FOREACH(curr, &serv->clients, __entries) {   \
        if (curr->socket == INVALID_SOCKET)             \
            continue;                                   \
        expr;                                           \
    }                                                   \

typedef struct server_s {
    fd_t                socket;
    fd_set              fd_wset;
    fd_set              fd_rset;
    clientq_t           clients;
    struct sockaddr_in  addr;
    uint16_t            nb_clients;
    socklen_t           socket_len;
    client_t            *new_client;
    double              timeout;
} server_t;

static inline void fill_timeval(struct timeval *tv, double seconds)
{
    tv->tv_sec  = (long)seconds;
    tv->tv_usec = (long)((seconds - tv->tv_sec) * 1e6);
}

nw_error_t init_server(server_t *serv, in_port_t port, double timeout);
void use_select(server_t *server);
int accept_client(server_t *server);
void destroy_server(server_t *server);
int disconnect_client(server_t *server, client_t *client);