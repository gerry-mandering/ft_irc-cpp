#pragma once

#include "Demultiplexer.hpp"
#include "EventHandler.hpp"
#include <sys/event.h>
#include <vector>

class KqueueDemultiplexer : public Demultiplexer
{
  private:
    int m_kq;
    std::vector<struct kevent> m_kEventList;
    std::vector<struct kevent> m_changeList;
    size_t m_changePos;

  public:
    KqueueDemultiplexer();
    ~KqueueDemultiplexer();

    void setNumHandlers(size_t nHandlers);
    virtual int registerEvent(EventHandler *handler, eventType type);
    virtual int unregisterEvent(EventHandler *handler, eventType type);
    virtual int waitEvents(std::map<handle_t, EventHandler *> &handlers);
};