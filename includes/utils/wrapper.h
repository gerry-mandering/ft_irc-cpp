#pragma once

#include <cerrno>
#include <cstring>
#include <netdb.h>
#include <sys/errno.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace Wrapper
{
int kqueue(void);
int getaddrinfo(const char *hostname, const char *servname, const struct addrinfo *hints, struct addrinfo **res);
int socket(int domain, int type, int protocol);
int setsockopt(int socket, int level, int option_name, void *opt_val, socklen_t opt_len);
int bind(int socket, const struct sockaddr *address, socklen_t address_len);
int listen(int socket, int backlog);
int accept(int socket, struct sockaddr *address, socklen_t *address_len);
} // namespace Wrapper
