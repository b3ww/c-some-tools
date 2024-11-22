/*
** EPITECH PROJECT, 2023
** zappy_server
** File description:
** network.h
*/

#pragma once
#include "message_queue.h"
#include <sys/types.h>
#include <netinet/in.h>

typedef enum {
    NO_ERR,
    FAILED_TO_BIND_PORT,
    FAILED_TO_CREATE_SOCKET,
    FAILED_TO_START_LISTNING,
    FAILED_TO_ACCEPT_CLIENT,
    FAILED_TO_READ_DATA,
    CLIENT_DISCONNECT,
    FAILED_TO_SEND_DATA,
    FAIL
} nw_error_t;

#define VALID_CODE(code) (code == NO_ERR)

typedef struct message_queue msgq_t;
typedef struct message msg_t;

#define MESSAGE(msg_s) msg_s->text;

#define INVALID_SOCKET -1

typedef int fd_t;

#define CLOSE_SOCKET(fd) do {close(fd); fd = INVALID_SOCKET;} while (0);

nw_error_t create_socket(in_port_t, struct sockaddr_in *, int, fd_t *);
