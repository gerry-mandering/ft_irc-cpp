#ifndef CONNECTION_INITIALIZER_H
#define CONNECTION_INITIALIZER_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include "../wrapper/Wrapper.hpp"
#include "../singleton_template/SingletonTemplate.hpp"
#include "../reactor_pattern/ReactorPattern.hpp"
#include "../reactor/Reactor.hpp"
#include "../request_handler/RequestHandler.hpp"

class ConnectionInitializer : public TSingleton<ConnectionInitializer>, public reactor::EventHandler {

public:
    ConnectionInitializer();
    ~ConnectionInitializer();

    void HandleRead();
    reactor::Socket GetSocket() const;

private:
    static const std::string PORT_NUMBER;
    static const int BACKLOG_QUEUE_SIZE = 5;

    struct addrinfo *mAddressInfo;
    reactor::Socket mListeningSocket;

};

#endif