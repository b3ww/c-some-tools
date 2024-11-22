/*
** EPITECH PROJECT, 2023
** zappy_server
** File description:
** socket.c
*/

#include "network.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "logger.h"

static void fill_address(struct sockaddr_in *address,
    in_port_t port)
{
    memset(address, 0, sizeof(*address));
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(port);
}

nw_error_t create_socket(in_port_t port, struct sockaddr_in *address,
    int nb_clients, fd_t *fd)
{
    int opt = 1;

    if ((*fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return FAILED_TO_CREATE_SOCKET;
    fill_address(address, port);
    if (setsockopt(*fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        DLOG_F("ERROR", "%s: %s", "Erreur de setsockopt", strerror(errno));
        return FAIL;
    }
    if (bind(*fd, (struct sockaddr *)address, sizeof(*address)) == -1) {
        CLOSE_SOCKET(*fd);
        return FAILED_TO_BIND_PORT;
    }
    if (listen(*fd, nb_clients) == -1) {
        CLOSE_SOCKET(*fd);
        return FAILED_TO_START_LISTNING;
    }
    return NO_ERR;
}
