#pragma once

#include <iostream>
#include "../reactor_pattern/ReactorPattern.hpp"
#include "../client/Client.hpp"
#include "../client_repository/ClientRepository.hpp"

typedef enum eRequestType {
    PASS,
    NICK,
    USER,
    PRIVMSG,
    JOIN,
    KICK,
    INVITE,
    QUIT,
    TOPIC,
    MODE,
    PING,
} eRequestType;

///////////////////////////////////////////////////////////////////////////////////

//Exception 정의도 각각 request에 할지 생각
class Request {

public:
    Request(reactor::Socket socket);

protected:
    virtual ~Request();

private:
    Client *mClient;

    eRequestType mRequestType;

};