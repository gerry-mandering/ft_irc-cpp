#pragma once

#include "Client.hpp"
#include "ClientRepository.hpp"
#include "EventHandler.hpp"
#include "LoggingHandler.hpp"
#include "VisitorPattern.hpp"
#include <iostream>
#include <sstream>

class Request
{

  public:
    Request(handle_t socket);

    virtual bool Accept(visitor_pattern::Visitor *visitor) = 0;

    Client *GetClient() const;

    friend std::ostream &operator<<(std::ostream &os, const Request &request);

  protected:
    virtual ~Request() {}

  private:
    handle_t mSocket;
    Client *mClient;
};
