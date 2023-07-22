#pragma once

#include "EventHandler.hpp"

class StreamSocket : public EventHandler
{

  public:
    StreamSocket(handle_t handle);
    virtual ~StreamSocket();

    virtual int handleRead();
    virtual int handleWrite();
    virtual int hadnleError();
};