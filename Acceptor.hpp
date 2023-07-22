#pragma once

#include "EventHandler.hpp"

class Acceptor : public EventHandler
{

  public:
    Acceptor(handle_t handle);
    virtual ~Acceptor();

    virtual int handleRead();
    virtual int handleWrite();
    virtual int handleError();
};