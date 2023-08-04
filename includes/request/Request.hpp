#pragma once

#include "Client.hpp"
#include "ClientRepository.hpp"
#include "EventHandler.hpp"
#include "LoggingHandler.hpp"
#include "SharedPtr.hpp"
#include "VisitorPattern.hpp"
#include <iostream>
#include <sstream>

class Request
{

  public:
    Request(handle_t socket);

    virtual bool Accept(visitor_pattern::Visitor *visitor) = 0;

    SharedPtr< Client > GetClient() const;

  protected:
    virtual ~Request() {}

  private:
    handle_t mSocket;
    SharedPtr< Client > mClient;
};
