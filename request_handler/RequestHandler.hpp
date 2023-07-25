#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <iostream>
#include <sys/socket.h>
#include "../reactor_pattern/ReactorPattern.hpp"

class RequestHandler : public reactor::EventHandler {

public:
    RequestHandler();
    ~RequestHandler();

    RequestHandler(const reactor::Socket clientSocket);

public:
    reactor::Socket GetSocket() const;
    void HandleRead();
    void HandleWrite();
    void HandleError();

private:
    reactor::Socket mClientSocket;

};

#endif