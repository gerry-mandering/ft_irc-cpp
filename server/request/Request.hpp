#pragma once

#include "Client.hpp"
#include "ClientRepository.hpp"
#include "EventHandler.hpp"
#include "VisitorPattern.hpp"
#include <iostream>

typedef enum eRequestType
{
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

// Exception 정의도 각각 request에 할지 생각
class Request
{

  public:
    Request(handle_t socket);

    virtual void Accept(visitor_pattern::Visitor *visitor) = 0;

    Client *GetClient() const;
    eRequestType GetType() const;

  protected:
    virtual ~Request() {}

  private:
    Client *mClient;
    eRequestType mRequestType;
};