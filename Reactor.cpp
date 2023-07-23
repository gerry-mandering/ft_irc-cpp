#pragma once

#include "Reactor.hpp"
#include "KqueueDemultiplexer.hpp"

DECLARE_SINGLETON_MEMBER(Reactor)

Reactor::Reactor() : m_numHandlers(0) {}

Reactor::~Reactor()
{
    delete m_demultiplexer;
}

void Reactor::setDemultiplexer(Demultiplexer *demultiplexer)
{
    m_demultiplexer = demultiplexer;
}

int Reactor::registerHandler(EventHandler *handler, eventType type)
{
    int handle = handler->getHandle();
    std::map<handle_t, EventHandler *>::iterator it = m_handlers.find(handle);

    if (it == m_handlers.end())
    {
        m_handlers[handle] = handler;
        ++m_numHandlers;
        m_demultiplexer->setnumHandlers(m_numHandlers);
    }
    return (registerEvent(handle, type));
}

int Reactor::unregisterHandler(EventHandler *handler)
{
    int handle = handler->getHandle();
    std::map<handle_t, EventHandler *>::iterator it = m_handlers.find(handle);
    int stat = 0;

    if (it != m_handlers.end())
    {
        m_handlers.erase(it);
        --m_numHandlers;
        m_demultiplexer->setnumHandlers(m_numHandlers);
    }
    stat |= unregisterEvent(handle, READ_EVENT);
    stat |= unregisterEvent(handle, WRITE_EVENT);
    return (stat);
}

int Reactor::registerEvent(handle_t handle, eventType type)
{
    return (m_demultiplexer->registerEvent(handle, type));
}

int Reactor::unregisterEvent(handle_t handle, eventType type)
{
    return (m_demultiplexer->unregisterEvent(handle, type));
}

int Reactor::handleEvents(void)
{
    return (m_demultiplexer->waitEvents(m_handlers));
}