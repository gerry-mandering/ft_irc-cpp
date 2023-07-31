#include "wrapper.h"
#include "LoggingHandler.hpp"
#include "SyscallException.hpp"

namespace Wrapper
{
inline ::std::string strerror(int errnum)
{
    return ::std::string(::std::strerror(errnum));
}

int kqueue(void)
{
    int fd = ::kqueue();

    if (fd < 0)
    {
        LOG_ERROR("kqueue systemcall failed " << strerror(errno));
        throw SyscallException("kqueue systemcall failed" + strerror(errno));
    }
    return fd;
}

int getaddrinfo(const char *hostname, const char *servname, const struct addrinfo *hints, struct addrinfo **res)
{
    if (::getaddrinfo(hostname, servname, hints, res) < 0)
    {
        LOG_ERROR("getaddrinfo systemcall failed " << strerror(errno));
        throw SyscallException("getaddrinfo systemcall failed " + strerror(errno));
    }
    return 0;
}

int socket(int domain, int type, int protocol)
{
    int fd = ::socket(domain, type, protocol);

    if (fd < 0)
    {
        LOG_ERROR("socket systemcall failed " << strerror(errno));
        throw SyscallException("socket systemcall failed " + strerror(errno));
    }
    return fd;
}

int setsockopt(int socket, int level, int option_name, void *opt_val, socklen_t opt_len)
{
    if (::setsockopt(socket, level, option_name, opt_val, opt_len) < 0)
    {
        LOG_ERROR("setsockopt systemcall failed " << strerror(errno));
        throw SyscallException("setsockopt systemcall failed " + strerror(errno));
    }
    return 0;
}

int bind(int socket, const struct sockaddr *address, socklen_t address_len)
{
    if (::bind(socket, address, address_len) < 0)
    {
        LOG_ERROR("bind systemcall failed " << strerror(errno));
        throw SyscallException("bind systemcall failed " + strerror(errno));
    }
    return 0;
}

int listen(int socket, int backlog)
{
    if (::listen(socket, backlog) < 0)
    {
        LOG_ERROR("listen systemcall failed " << strerror(errno));
        throw SyscallException("listen systemcall failed " + strerror(errno));
    }
    return 0;
}

int accept(int socket, struct sockaddr *address, socklen_t *address_len)
{
    int fd = ::accept(socket, address, address_len);

    if (fd < 0)
    {
        LOG_ERROR("accept systemcall failed " << strerror(errno));
        throw SyscallException("accept systemcall failed " + strerror(errno));
    }
    return fd;
}
} // namespace Wrapper
