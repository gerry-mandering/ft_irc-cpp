#include "KqueueDemultiplexer.hpp"
#include "LoggingHandler.hpp"
#include "SyscallException.hpp"
#include "color.h"
#include "disconnect.h"
#include "wrapper.h"
#include <iostream>
#include <unistd.h>

KqueueDemultiplexer::KqueueDemultiplexer() : m_kEventList(8), m_changeList(16), m_changePos(0), m_numHandlers(0)
{
    m_kq = Wrapper::kqueue();
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

int KqueueDemultiplexer::registerEvent(EventHandler *handler, eEventType type)
{
    if (type == OFF_EVENT)
        return CODE_OK;
    // hanlder가 이미 이벤트를 감시중이라면 굳이 새로 등록하지 않는다
    if (handler->getEventFlag() & type)
        return (CODE_OK);
    if (m_changePos >= m_changeList.size())
        m_changeList.resize(m_changeList.size() * 2);
    struct kevent &event = m_changeList[m_changePos++];
    int handle = handler->getHandle();
    if (type & READ_EVENT)
        EV_SET(&event, handle, EVFILT_READ, EV_ADD, 0, 0, handler);
    if (type & WRITE_EVENT)
        EV_SET(&event, handle, EVFILT_WRITE, EV_ADD, 0, 0, handler);
    eEventType newFlag = static_cast< eEventType >(handler->getEventFlag() | type);
    handler->setEventFlag(newFlag);
    return (CODE_OK);
}

int KqueueDemultiplexer::unregisterEvent(EventHandler *handler, eEventType type)
{
    // handler가 이미 이벤트를 감시하고 있지 않다면 굳이 삭제하지 않는다
    if ((handler->getEventFlag() & type) == OFF_EVENT)
        return (CODE_OK);
    if (m_changePos >= m_changeList.size())
        m_changeList.resize(m_changeList.size() * 2);
    struct kevent &event = m_changeList[m_changePos++];
    int handle = handler->getHandle();
    if (type & READ_EVENT)
        EV_SET(&event, handle, EVFILT_READ, EV_DELETE, 0, 0, 0);
    if (type & WRITE_EVENT)
        EV_SET(&event, handle, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
    eEventType newFlag = static_cast< eEventType >(handler->getEventFlag() & ~type);
    handler->setEventFlag(newFlag);
    return (CODE_OK);
}

int KqueueDemultiplexer::waitEvents(std::map< handle_t, EventHandler * > &handlers)
{
    int numEvents = kevent(m_kq, &m_changeList[0], m_changePos, &m_kEventList[0], m_kEventList.size(), NULL);
    m_changePos = 0;
    if (numEvents < 0)
        return (numEvents);
    for (int i = 0; i < numEvents; ++i)
    {
        struct kevent &event = m_kEventList[i];
        EventHandler *handler = reinterpret_cast< EventHandler * >(event.udata);

        // 클라이언트 프로세스가 종료되면 해당 플래그로 온다
        if (event.flags & EV_EOF || event.flags & EV_ERROR)
        {
            handler->handleDisconnect();
            Reactor::GetInstance()->unregisterHandler(handler);
            continue;
        }
        if (event.filter == EVFILT_READ)
        {
            if (handler->handleRead() == CLIENT_DISCONNECT)
                Reactor::GetInstance()->unregisterHandler(handler);
        }
        if (event.filter == EVFILT_WRITE)
            handler->handleWrite();
    }
    (void)handlers;
    return (numEvents);
}
