#pragma once

#include "EventHandler.hpp"
#include <map>

class Demultiplexer
{
  protected:
    size_t m_numHandlers;

  public:
    Demultiplexer();
    virtual ~Demultiplexer();

    virtual int registerEvent(EventHandler *handle, eventType type) = 0;
    virtual int unregisterEvent(EventHandler *handle, eventType type) = 0;
    virtual int waitEvents(std::map<handle_t, EventHandler *> &handlers) = 0;
    virtual void setNumHandlers(size_t nHandlers);
};
