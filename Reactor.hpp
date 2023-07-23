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

    void setDemultiplexer(Demultiplexer *demultiplexer);
    std::map<handle_t, EventHandler *> Reactor::getHandlers();

    int registerHandler(EventHandler *handler, eventType type);
    int unregisterHandler(EventHandler *handler);
    int registerEvent(handle_t handle, eventType type);
    int unregisterEvent(handle_t handle, eventType type);
    int handleEvents(void);
};