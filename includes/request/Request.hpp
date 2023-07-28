#pragma once

#include "Client.hpp"
#include "ClientRepository.hpp"
#include "EventHandler.hpp"
#include "VisitorPattern.hpp"
#include <iostream>

class Request
{

  public:
    Request(handle_t socket);

    virtual void Accept(visitor_pattern::Visitor *visitor) = 0;

    Client *GetClient() const;

  protected:
    virtual ~Request() {}

  private:
    handle_t mSocket;
    Client *mClient;
};