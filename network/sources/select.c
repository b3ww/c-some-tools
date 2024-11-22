/*
** EPITECH PROJECT, 2023
** zappy_server
** File description:
** select.c
*/

#include "server.h"
#include "client.h"
#include <sys/select.h>
#include "logger.h"
#include <string.h>
#include <unistd.h>

static void handle_read(fd_set *fd_rset, server_t *server)
{
    FOREACH_VALID_CLIENT(server, client,
        if (FD_ISSET((client->socket), (fd_rset))) {
            if (client_read(client) != NO_ERR) {
                disconnect_client(server, client);
            }
        }
    );
}

static void handle_write(fd_set *fd_wset __attribute__((__unused__)), server_t *server)
{
    FOREACH_VALID_CLIENT(server, client,
        if (FD_ISSET(client->socket, fd_wset)) {
            switch (client_write(client)) {
                case NO_ERR:
                    break;
                default:
                    disconnect_client(server, client);
                    break;
            }
        }
    );
}

static void handle_except(fd_set *fd_eset, server_t *server)
{
    FOREACH_VALID_CLIENT(server, client,
        if (FD_ISSET(client->socket, fd_eset)) {
            LOG_F("ERROR", "Exception on client socket (%d)", client->socket);
            disconnect_client(server, client);
        }
    );
}

static fd_t setup_fd(server_t *server, fd_set *fd_eset)
{
    fd_t fd_max = server->socket;

    server->new_client = NULL;
    FD_ZERO(&server->fd_rset);
    FD_ZERO(&server->fd_wset);
    FD_ZERO(fd_eset);
    FD_SET(server->socket, &server->fd_rset);
    FD_SET(server->socket, &server->fd_wset);
    FD_SET(server->socket, fd_eset);
    FOREACH_VALID_CLIENT(server, client,
        FD_SET(client->socket, &server->fd_rset);
        FD_SET(client->socket, fd_eset);
        if (have_message(&client->out_msg)) {
            FD_SET(client->socket, &server->fd_wset);
        }
        fd_max = client->socket > fd_max ? client->socket : fd_max;
    );
    return fd_max;
}

void use_select(server_t *server)
{
    int activity = 0;
    struct timeval timeout;
    fd_set fd_eset;

    fill_timeval(&timeout, server->timeout);
    activity = select(setup_fd(server, &fd_eset) + 1, &server->fd_rset, &server->fd_wset, &fd_eset,
        (server->timeout && server->nb_clients) ? &timeout : NULL);
    if (activity == -1) {
        LOG_F("ERROR", "Socket server error (%s) -> %d", strerror(errno), server->socket);
        return;
    }
    if (activity <= 0)
        return;
    if (FD_ISSET(server->socket, &server->fd_rset)) {
        switch (accept_client(server)) {
            case FAILED_TO_ACCEPT_CLIENT: break;
            default: break;
        }
    }
    handle_read(&server->fd_rset, server);
    handle_write(&server->fd_wset, server);
    handle_except(&fd_eset, server);
}
