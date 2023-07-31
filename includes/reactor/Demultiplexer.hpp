#pragma once

#include "EventHandler.hpp"
#include <map>

class Demultiplexer
{
  public:
    Demultiplexer() {}
    virtual ~Demultiplexer() {}

    virtual int registerEvent(EventHandler *handle, eEventType type) = 0;
    virtual int unregisterEvent(EventHandler *handle, eEventType type) = 0;
    virtual int waitEvents(std::map<handle_t, EventHandler *> &handlers) = 0;
    virtual void setNumHandlers(size_t nHandlers) { (void)nHandlers; }
};
