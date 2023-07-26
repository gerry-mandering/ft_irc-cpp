#pragma once

#include <iostream>
#include "EventHandler.hpp"
#include "Client.hpp"
#include "ClientRepository.hpp"
#include "Command.hpp"
#include "Validator.hpp"

typedef enum eRequestType {
    CAP,
    INVITE,
    JOIN,
    KICK,
    MODE,
    NICK,
    PART,
    PASS,
    PING,
    PRIVMSG,
    QUIT,
    TOPIC,
    USER,
} eRequestType;

///////////////////////////////////////////////////////////////////////////////////

//Exception 정의도 각각 request에 할지 생각
class Request {

public:
    Request(handle_t socket);

    virtual void AcceptValidator(Validator *validator) = 0;
    virtual void AcceptCommand(Command *command) = 0;

    Client *GetClient() const;
    eRequestType GetType() const;

protected:
    virtual ~Request() {}

private:
    Client *mClient;
    eRequestType mRequestType;

};