//
// Created by Karpukhin Aleksandr on 30.01.2022.
//

#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include "errno.h"

#include "defines.h"

void print_to_buf(char *restrict buf, const char *prefix, const char *suffix) {
    if (prefix == NULL) {
        fprintf(stderr, ERR_NO_ERR_MESSAGE_MESSAGE);
        exit(ERR_NO_ERR_MESSAGE);
    }

    if (suffix == NULL) {
        snprintf(buf, ERROR_MESSAGE_MAX_LENGTH, "%s", prefix);
    } else {
        snprintf(buf, ERROR_MESSAGE_MAX_LENGTH, "%s: %s", prefix, suffix);
    }
}

void log_error(err_code_t code, const char *message) {
    char buf[ERROR_MESSAGE_MAX_LENGTH];

    switch (code) {
        case ERR_NOT_ALLOCATED:
            print_to_buf(buf, ERR_NOT_ALLOCATED_MESSAGE, message);
            break;
        case ERR_OUT_OF_RANGE:
            print_to_buf(buf, ERR_OUT_OF_RANGE_MESSAGE, message);
            break;
        case ERR_INVALID_ARGV:
            print_to_buf(buf, ERR_INVALID_ARGV_MESSAGE, message);
            break;
        case ERR_NULL_POINTER:
            print_to_buf(buf, ERR_NULL_POINTER_MESSAGE, message);
            break;
        case ERR_PARSE_CONFIG:
            print_to_buf(buf, ERR_PARSE_CONFIG_MESSAGE, message);
            break;
        case ERR_PARSE_PORT:
            print_to_buf(buf, ERR_PARSE_PORT_MESSAGE, message);
            break;
        case ERR_PARSE_PROC_NUM:
            print_to_buf(buf, ERR_PARSE_PROC_NUM_MESSAGE, message);
            break;
        case ERR_PARSE_BACKLOG:
            print_to_buf(buf, ERR_PARSE_BACKLOG_MESSAGE, message);
            break;
        case ERR_SOCKET_INIT:
            print_to_buf(buf, ERR_SOCKET_INIT_MESSAGE, message);
            break;
        case ERR_SOCKET_BIND:
            print_to_buf(buf, ERR_SOCKET_BIND_MESSAGE, message);
            break;
        case ERR_SOCKET_LISTEN:
            print_to_buf(buf, ERR_SOCKET_LISTEN_MESSAGE, message);
            break;
        case ERR_SOCKET_ACCEPT:
            print_to_buf(buf, ERR_SOCKET_ACCEPT_MESSAGE, message);
            break;
        case ERR_SOCKET_POLL:
            print_to_buf(buf, ERR_SOCKET_POLL_MESSAGE, message);
        case ERR_SOCKET_POLL_POLLERR:
            print_to_buf(buf, ERR_SOCKET_POLL_POLLERR_MESSAGE, message);
        case ERR_UNKNOWN:
            print_to_buf(buf, ERR_UNKNOWN_MESSAGE, message);
            break;
        default:
            print_to_buf(buf, message, NULL);
    }

    errno = code;
    fprintf(stderr, "\n%s\n", buf);
}

void log_error_code(err_code_t code) {
    log_error(code, NULL);
}

void log_error_message(err_code_t code, const char *message) {
    log_error(code, message);
}

void exit_with_error_message(err_code_t code, const char *message) {
    log_error(code, message);
    exit(code);
}

void exit_with_error_code(err_code_t code) {
    exit_with_error_message(code, NULL);
}

void exit_with_config_error(const config_t *config, const char * filename) {
    const char *text = config_error_text(config);
    int line = config_error_line(config);
    char buf[ERROR_MESSAGE_MAX_LENGTH];

    snprintf(buf, ERROR_MESSAGE_MAX_LENGTH, "%s:%d - %s", filename, line, text);
    exit_with_error_message(ERR_PARSE_CONFIG, buf);
}

