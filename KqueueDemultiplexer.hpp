#pragma once

#include "Demultiplexer.hpp"
#include <sys/event.h>
#include <vector>

class KqueueDemultiplexer : public Demultiplexer
{
  public:
    KqueueDemultiplexer();
    ~KqueueDemultiplexer();

    virtual int waitEvents(std::map<handle_t, EventHandler *> &handlers);
    virtual int registerEvent(handle_t handle, eventType et);
    virtual int unregisterEvent(handle_t handle, eventType et);

  private:
    int m_kq;
    std::vector<struct kevent> m_kEventList;
    std::vector<struct kevent> m_changeList;
    int m_changePos;
};