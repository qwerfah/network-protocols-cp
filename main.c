#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <unistd.h>


#include "config.h"
#include "error.h"
#include "defines.h"
#include "server-fsm.h"


pollfd_t pollfd_ctr(int fd, short events) {
    pollfd_t p = {fd, events, 0};
    return p;
}

te_smtp_server_fsm_state serve_connection(pollfd_t pollfd, te_smtp_server_fsm_state conn_state) {
    if (pollfd.revents & POLLIN) {

    } else if (pollfd.revents & POLLHUP) {
        return smtp_server_fsm_step(conn_state, SMTP_SERVER_FSM_EV_CONN_LOST, NULL, NULL);
    } else if (pollfd.revents & POLLERR) {
        char buf[100];
        snprintf(buf, 100, "socket_fd=%d", pollfd.fd);
        log_error_message(ERR_SOCKET_POLL_POLLERR, buf);
    }
}

int main(int argc, char *argv[]) {
    server_config_t server_config;
    int server_socket;
    sockaddr_in_t server_addr;

    parse_app_config(&server_config, argc, argv);

    printf("config:\n");
    printf("host: %s\n", server_config.host);
    printf("port: %d\n", server_config.port);
    printf("user: %s\n", server_config.user);
    printf("group: %s\n", server_config.group);
    printf("proc: %d\n", server_config.proc_count);
    printf("domain: %s\n", server_config.domain);
    printf("maildir: %s\n", server_config.maildir);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        exit_with_error_code(ERR_SOCKET_INIT);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_config.port);
    inet_aton(server_config.host, &server_addr.sin_addr);

    if (bind(server_socket, (sockaddr_t *) &server_addr, sizeof(server_addr)) < 0) {
        exit_with_error_code(ERR_SOCKET_BIND);
    }

    if (listen(server_socket, server_config.backlog) < 0) {
        exit_with_error_code(ERR_SOCKET_LISTEN);
    }

    pollfd_t *socket_pool = NULL;
    te_smtp_server_fsm_state *conn_states = NULL;
    size_t socket_pool_size = 0;
    size_t socket_pool_capacity = 0;
    size_t last_free_pos = -1;

    while (TRUE) {
        sockaddr_in_t client_addr;
        socklen_t addr_len;
        int client_socket;

        // accept new connection
        if ((client_socket = accept(server_socket, (sockaddr_t *) &client_addr, &addr_len)) < 0) {
            log_error_code(ERR_SOCKET_ACCEPT);
        } else if (last_free_pos != -1) {
            socket_pool[last_free_pos] = pollfd_ctr(client_socket, POLL_FD_EVENTS);
            conn_states[last_free_pos] = smtp_server_fsm_step(SMTP_SERVER_FSM_ST_INIT,
                                                              SMTP_SERVER_FSM_EV_CONN_EST,
                                                              NULL,
                                                              NULL);
            last_free_pos = -1;
        } else if (socket_pool_size == socket_pool_capacity) {
            socket_pool_capacity += SOCKET_POOL_CHUNK;

            if ((socket_pool = (pollfd_t *) realloc(
                    socket_pool, sizeof(pollfd_t) * socket_pool_capacity)) == NULL) {
                exit_with_error_message(ERR_NOT_ALLOCATED, "can't extend memory for client sockets pool");
            }
            if ((conn_states = (te_smtp_server_fsm_state *) realloc(
                    conn_states, sizeof(te_smtp_server_fsm_state) * socket_pool_capacity)) == NULL) {
                exit_with_error_message(ERR_NOT_ALLOCATED, "can't extend memory for fsm states array");
            }
        } else {
            socket_pool[socket_pool_size] = pollfd_ctr(client_socket, POLL_FD_EVENTS);
            conn_states[socket_pool_size++] = smtp_server_fsm_step(SMTP_SERVER_FSM_ST_INIT,
                                                                   SMTP_SERVER_FSM_EV_CONN_EST,
                                                                   NULL,
                                                                   NULL);
        }

        // poll all sockets for actions
        int ready_count = poll(socket_pool, socket_pool_size, DEFAULT_POLL_TIMEOUT_MS);
        if (ready_count < 0) {
            exit_with_error_code(ERR_SOCKET_POLL);
        }

        for (int i = 0; i < socket_pool_size && ready_count > 0; i++) {
            if (socket_pool[i].revents) {
                if (conn_states[i] = serve_connection(socket_pool[i], conn_states[i]) == SMTP_SERVER_FSM_ST_DONE) {
                    if (close(socket_pool[i].fd) < 0) {
                        log_error_code(ERR_SOCKET_ACCEPT);
                    }
                    socket_pool[i].fd = -1;
                    last_free_pos = i;
                }
                ready_count--;
            }
        }
    }

    exit(0);
}
