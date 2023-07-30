#pragma once

#include "Demultiplexer.hpp"
#include "EventHandler.hpp"
#include "Singleton.hpp"
#include <map>

class Reactor : public Singleton<Reactor>
{
  private:
    Demultiplexer *m_demultiplexer;
    std::map<handle_t, EventHandler *> m_handlers;
    int m_numHandlers;

  public:
    Reactor();
    ~Reactor();

    EventHandler *getHandler(handle_t handle) const;
    void setDemultiplexer(Demultiplexer *demultiplexer);

    int registerHandler(EventHandler *handler, eEventType type);
    int unregisterHandler(EventHandler *handler);
    int registerEvent(EventHandler *handler, eEventType type);
    int unregisterEvent(EventHandler *handler, eEventType type);
    int handleEvents(void);
};