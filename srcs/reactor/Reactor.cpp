#include "Reactor.hpp"
#include "KqueueDemultiplexer.hpp"
#include <iostream>

Reactor::Reactor() : m_numHandlers(0) {}

Reactor::~Reactor()
{
    delete m_demultiplexer;
}

EventHandler *Reactor::getHandler(handle_t handle) const
{
    std::map< handle_t, EventHandler * >::const_iterator it = m_handlers.find(handle);

    if (it == m_handlers.end())
        return (NULL);
    return (it->second);
}

void Reactor::setDemultiplexer(Demultiplexer *demultiplexer)
{
    m_demultiplexer = demultiplexer;
}

int Reactor::registerHandler(EventHandler *handler, eEventType type)
{
    int handle = handler->getHandle();
    std::map< handle_t, EventHandler * >::iterator it = m_handlers.find(handle);

    if (it == m_handlers.end())
    {
        m_handlers[handle] = handler;
        ++m_numHandlers;
        m_demultiplexer->setNumHandlers(m_numHandlers);
    }
    return (registerEvent(handler, type));
}

int Reactor::unregisterHandler(EventHandler *handler)
{
    int handle = handler->getHandle();
    std::map< handle_t, EventHandler * >::iterator it = m_handlers.find(handle);
    int stat = 0;

    if (it != m_handlers.end())
    {
        m_handlers.erase(it);
        --m_numHandlers;
        m_demultiplexer->setNumHandlers(m_numHandlers);
    }
    stat |= unregisterEvent(handler, READ_EVENT);
    stat |= unregisterEvent(handler, WRITE_EVENT);
    return (stat);
}

int Reactor::registerEvent(EventHandler *handler, eEventType type)
{
    return (m_demultiplexer->registerEvent(handler, type));
}

int Reactor::unregisterEvent(EventHandler *handler, eEventType type)
{
    return (m_demultiplexer->unregisterEvent(handler, type));
}

int Reactor::handleEvents(void)
{
    return (m_demultiplexer->waitEvents(m_handlers));
}