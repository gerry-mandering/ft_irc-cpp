#ifndef WRAPPER_H
#define WRAPPER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "../exception/Exception.hpp"

namespace wrapper {

    int wGetaddrinfo(const char *hostname, const char *servname, const struct addrinfo *hints, struct addrinfo **res);
    int wSocket(int domain, int type, int protocol);
    int wBind(int socket, const struct sockaddr *address, socklen_t address_len);
    int wListen(int socket, int backlog);

}

#endif