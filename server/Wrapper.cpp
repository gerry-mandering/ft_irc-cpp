#include "Wrapper.hpp"

int
wrapper::wGetaddrinfo(const char *hostname, const char *servname, const struct addrinfo *hints, struct addrinfo **res) {
    if (getaddrinfo(hostname, servname, hints, res) != 0) {
        throw AddrInfoException();
    }
    return 0;
}

int wrapper::wSocket(int domain, int type, int protocol) {
    int ret = socket(domain, type, protocol);

    if (ret == -1) {
        throw SocketCreationException();
    }

    return ret;
}

int wrapper::wBind(int socket, const struct sockaddr *address, socklen_t address_len) {
    if (bind(socket, address, address_len) != 0) {
        throw BindException();
    }

    return 0;
}

int wrapper::wListen(int socket, int backlog) {
    if (listen(socket, backlog) != 0) {
        throw ListenException();
    }

    return 0;
}
