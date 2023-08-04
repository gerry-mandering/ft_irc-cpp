#include "KqueueDemultiplexer.hpp"
#include "LoggingHandler.hpp"
#include "SyscallException.hpp"
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

static std::string eventTypeToString(eEventType type)
{
    if (type == READ_EVENT)
        return ("READ_EVENT");
    if (type == WRITE_EVENT)
        return ("WRITE_EVENT");
    return ("UNKNOWN");
}

int KqueueDemultiplexer::registerEvent(EventHandler *handler, eEventType type)
{
    // hanlder가 이미 이벤트를 감시중이라면 굳이 새로 등록하지 않는다
    if (handler->getEventFlag() & type)
    {
        LOG_WARN(__func__ << " [ " << handler->getHandle() << " ] already monitor " << eventTypeToString(type));
        return (CODE_OK);
    }
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
    LOG_DEBUG("[ " << handler->getHandle() << " ] register " << eventTypeToString(type));
    return (CODE_OK);
}

int KqueueDemultiplexer::unregisterEvent(EventHandler *handler, eEventType type)
{
    // handler가 이미 이벤트를 감시하고 있지 않다면 굳이 삭제하지 않는다
    if ((handler->getEventFlag() & type) == OFF_EVENT)
    {
        LOG_WARN(__func__ << " [ " << handler->getHandle() << " ] already not monitor " << eventTypeToString(type));
        return (CODE_OK);
    }
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
    LOG_DEBUG("[ " << handler->getHandle() << " ] unregister " << eventTypeToString(type));
    return (CODE_OK);
}

int KqueueDemultiplexer::waitEvents(std::map< handle_t, EventHandler * > &handlers)
{
    int numEvents = kevent(m_kq, &m_changeList[0], m_changePos, &m_kEventList[0], m_kEventList.size(), NULL);
    m_changePos = 0;
    LOG_TRACE("\n>====== numEvents: " << numEvents << " ======<\n");
    if (numEvents < 0)
        return (numEvents);
    for (int i = 0; i < numEvents; ++i)
    {
        struct kevent &event = m_kEventList[i];
        EventHandler *handler = reinterpret_cast< EventHandler * >(event.udata);

        LOG_TRACE("event ident: " << event.ident);
        // 클라이언트 프로세스가 종료되면 해당 플래그로 온다
        if (event.flags & EV_EOF || event.flags & EV_ERROR)
        {
            // TODO: EV_EOF 플래그 검사(예외처리), disconnect 여기에서 호출
            LOG_TRACE(__func__ << " EOF event");
            close(event.ident);
            // handler->handleDisconnect();
            continue;
        }
        if (event.filter == EVFILT_READ)
        {
            LOG_TRACE(__func__ << " READ event");
            handler->handleRead();
        }
        if (event.filter == EVFILT_WRITE)
        {
            LOG_TRACE(__func__ << " WRITE event");
            handler->handleWrite();
        }
        LOG_TRACE("event loop");
    }
    (void)handlers;
    return (numEvents);
}
