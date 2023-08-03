#pragma once

#include "Demultiplexer.hpp"
#include "EventHandler.hpp"
#include <sys/event.h>
#include <vector>

class KqueueDemultiplexer : public Demultiplexer
{
  private:
    int m_kq;
    std::vector< struct kevent > m_kEventList;
    std::vector< struct kevent > m_changeList;
    size_t m_changePos;
    size_t m_numHandlers;

  public:
    KqueueDemultiplexer();
    ~KqueueDemultiplexer();

    virtual void setNumHandlers(size_t nHandlers);
    virtual int registerEvent(EventHandler *handler, eEventType type);
    virtual int unregisterEvent(EventHandler *handler, eEventType type);
    virtual int waitEvents(std::map< handle_t, EventHandler * > &handlers);
};