#include "KqueueDemultiplexer.hpp"
#include "def.h"
#include <unistd.h>

#include <iostream>

KqueueDemultiplexer::KqueueDemultiplexer()
    : m_kEventList(8), m_changeList(16), m_changePos(0)
{
    // TODO: 예외처리
    m_kq = kqueue();
    assert(m_kq != -1);
}

KqueueDemultiplexer::~KqueueDemultiplexer()
{
    close(m_kq);
}

void KqueueDemultiplexer::setNumHandlers(size_t nHandlers)
{
    m_numHandlers = nHandlers;
    if (m_numHandlers > m_kEventList.size())
        m_kEventList.resize(m_numHandlers * 2);
}

int KqueueDemultiplexer::registerEvent(EventHandler *handler, eventType et)
{
    if (m_changePos >= m_changeList.size())
        m_changeList.resize(m_changeList.size() * 2);
    struct kevent &event = m_changeList[m_changePos++];
    int handle = handler->getHandle();
    if (et & READ_EVENT)
        EV_SET(&event, handle, EVFILT_READ, EV_ADD, 0, 0, handler);
    if (et & WRITE_EVENT)
        EV_SET(&event, handle, EVFILT_WRITE, EV_ADD, 0, 0, handler);
    return (OK);
}

// TODO: changlist에 EV_ADD, EV_DEL 두가지 kevent가 등록되있는 경우 처리
int KqueueDemultiplexer::unregisterEvent(EventHandler *handler, eventType et)
{
    if (m_changePos >= m_changeList.size())
        m_changeList.resize(m_changeList.size() * 2);
    struct kevent &event = m_changeList[m_changePos++];
    int handle = handler->getHandle();
    if (et & READ_EVENT)
        EV_SET(&event, handle, EVFILT_READ, EV_DELETE, 0, 0, 0);
    if (et & WRITE_EVENT)
        EV_SET(&event, handle, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
    return (OK);
}

int KqueueDemultiplexer::waitEvents(
    std::map<handle_t, EventHandler *> &handlers)
{
    // TODO: kevent 반환값 예외처리
    int nEvents = kevent(m_kq, &m_changeList[0], m_changePos, &m_kEventList[0],
                         m_kEventList.size(), NULL);
    m_changePos = 0;
    assert(nEvents != -1);
    // TODO: EV_EOF 플래그 검사(예외처리)
    for (int i = 0; i < nEvents; ++i)
    {
        struct kevent &event = m_kEventList[i];
        EventHandler *handler = reinterpret_cast<EventHandler *>(event.udata);
        if (event.flags & EV_EOF)
        {
            std::cerr << "EOF event\n";
            close(event.ident);
            continue;
        }
        if (event.filter == EVFILT_READ)
        {
            std::cerr << "read event\n";
            handler->handleRead();
        }
        if (event.filter == EVFILT_WRITE)
        {
            std::cerr << "write event\n";
            handler->handleWrite();
        }
        std::cerr << "in event\n";
    }
    (void)handlers;
    return (nEvents);
}
