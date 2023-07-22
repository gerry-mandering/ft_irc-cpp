#pragma once

#include "EventHandler.hpp"
#include "Singleton.hpp"
#include "Demultiplexer.hpp"
#include <map>

class Reactor : public Singleton<Reactor>
{
  private:
    Demultiplexer *m_demultiplexer;
    std::map<handle_t, EventHandler *> m_handlers;

  public:
    Reactor();
    ~Reactor();

    int registerHandler(EventHandler *eh, eventType et);
    int removeHandler(EventHandler *eh, eventType et);
    int handleEvents();
};