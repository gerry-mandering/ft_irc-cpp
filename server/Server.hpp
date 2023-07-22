#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "SingletonTemplate.hpp"
#include "ReactorPattern.h"
#include "Reactor.hpp"
#include "RequestHandler.hpp"

// Singleton으로 변경 할 것
class Server : public TSingleton<Server>, public reactor::EventHandler {
private:
    static const std::string PORT_NUMBER;
    static const int BACKLOG_QUEUE_SIZE = 5;

public:
    Server();
    ~Server();
    Server(const Server &other);
    Server &operator=(const Server &server);

public:
    reactor::Socket GetSocket() const;
    void HandleRead();

private:
    struct addrinfo *mServerAddress;
    reactor::Socket mListeningSocket;

private:
    void initServerAddress();
    void initConnectionRequestSocket();

};

#endif