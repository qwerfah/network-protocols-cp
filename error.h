//
// Created by Karpukhin Aleksandr on 30.01.2022.
//

#ifndef LAB01_ERROR_H
#define LAB01_ERROR_H

#include <libconfig.h>

// Memory access errors
#define ERR_NOT_ALLOCATED  -100
#define ERR_OUT_OF_RANGE   -101
#define ERR_NULL_POINTER   -102

// Parsing errors
#define ERR_PARSE_CONFIG   -200
#define ERR_PARSE_PORT     -201
#define ERR_PARSE_PROC_NUM -202
#define ERR_PARSE_BACKLOG  -203

// Socket ops errors
#define ERR_SOCKET_INIT    -300
#define ERR_SOCKET_BIND    -301
#define ERR_SOCKET_LISTEN  -302
#define ERR_SOCKET_ACCEPT  -303
#define ERR_SOCKET_POLL    -304
#define ERR_SOCKET_POLL_POLLERR   -305

#define ERR_INVALID_ARGV   -400
#define ERR_NO_ERR_MESSAGE -401

#define ERR_UNKNOWN        -500

#define ERR_NOT_ALLOCATED_MESSAGE           "error during memory allocation"
#define ERR_OUT_OF_RANGE_MESSAGE            "ERROR DURING ACCESS OUT OF RANGE"

#define ERR_NULL_POINTER_MESSAGE            "ACCESS TO NULL POINTER"
#define ERR_PARSE_CONFIG_MESSAGE            "ERROR DURING APPLICATION CONFIG PARSING"
#define ERR_PARSE_PORT_MESSAGE              "socket port must be in range [0, 65353]"
#define ERR_PARSE_PROC_NUM_MESSAGE          "proc num must be in range [0, 1024]"
#define ERR_PARSE_BACKLOG_MESSAGE           "backlog size must be strict positive int value"

#define ERR_INVALID_ARGV_MESSAGE            "INVALID COMMAND LINE ARGUMENTS"
#define ERR_NO_ERR_MESSAGE_MESSAGE          "no error message was specified"

#define ERR_SOCKET_INIT_MESSAGE             "can't create server socket"
#define ERR_SOCKET_BIND_MESSAGE             "can't bind server socket"
#define ERR_SOCKET_LISTEN_MESSAGE           "can't listen on server socket"
#define ERR_SOCKET_ACCEPT_MESSAGE           "error during server socket accept call"
#define ERR_SOCKET_POLL_MESSAGE             "error during poll call on client sockets"
#define ERR_SOCKET_POLL_POLLERR_MESSAGE     "poll call set POLLERR in revents"


#define ERR_UNKNOWN_MESSAGE "UNKNOWN ERROR CODE RECEIVED"

typedef int err_code_t;

void log_error_code(err_code_t code);
void log_error_message(err_code_t code, const char *message);

void exit_with_error_code(err_code_t code);
void exit_with_error_message(err_code_t code, const char *message);
void exit_with_config_error(const config_t *config, const char * filename);

#endif //LAB01_ERROR_H
