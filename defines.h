//
// Created by Karpukhin Aleksandr on 14.02.2022.
//

#ifndef NETWORK_PROTOCOLS_CP_DEFINES_H
#define NETWORK_PROTOCOLS_CP_DEFINES_H

#include <sys/poll.h>

#define OP_SUCCESS 0

#define ERROR_MESSAGE_MAX_LENGTH 1000
#define DEFAULT_CONFIG_PATH "../server.cfg"

#define TRUE 1
#define FALSE 0

#define SOCKET_POOL_CHUNK 10

#define POLL_FD_EVENTS POLLIN | POLLPRI | POLLOUT | POLLERR

#define DEFAULT_POLL_TIMEOUT_MS 500

typedef struct pollfd pollfd_t;
typedef struct sockaddr sockaddr_t;
typedef struct sockaddr_in sockaddr_in_t;
#endif //NETWORK_PROTOCOLS_CP_DEFINES_H
