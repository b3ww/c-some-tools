/*
** EPITECH PROJECT, 2023
** zappy_server
** File description:
** server.c
*/

#include <stdlib.h>
#include <sys/select.h>
#include <signal.h>
#include "server.h"
#include "logger.h"
#include <unistd.h>

static void catch_sig_pipe(int _ __attribute__((__unused__)))
{
    LOG_F("ERROR", "SIGPIPE: %s", strerror(errno));
}

void destroy_server(server_t *server)
{
    client_t *client = NULL;
    STAILQ_FOREACH(client, &server->clients, __entries) {
        msg_t *msg = NULL;
        buffer_t *buffer = NULL;
        if (client->socket != INVALID_SOCKET)
            close(client->socket);
        while ((msg = dequeue_message(&client->in_msg))) {
            free_message(msg);
        }
        while ((msg = dequeue_message(&client->out_msg))) {
            free_message(msg);
        }
        while ((buffer = dequeue_buffer(&client->buffer))) {
            free(buffer);
        }
    }
    while ((client = STAILQ_FIRST(&server->clients))) {
        STAILQ_REMOVE(&server->clients, client, client_s, __entries);
        free(client);
    }
    close(server->socket);
}

nw_error_t init_server(server_t *serv, in_port_t port, double timeout)
{
    nw_error_t code = create_socket(port, &serv->addr, 128, &serv->socket);

    LOG_F("NETWORK", "Select how many : %d", FD_SETSIZE);
    signal(SIGPIPE, catch_sig_pipe);
    serv->timeout = timeout;
    if(!VALID_CODE(code))
        return code;
    STAILQ_INIT(&serv->clients);
    serv->nb_clients = 0;
    serv->socket_len = sizeof(struct sockaddr_in);
    serv->new_client = NULL;
    FD_ZERO(&serv->fd_wset);
    FD_ZERO(&serv->fd_rset);
    return code;
}
