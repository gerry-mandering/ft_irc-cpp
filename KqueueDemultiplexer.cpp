#include "KqueueDemultiplexer.hpp"

KqueueDemultiplexer::KqueueDemultiplexer()
    : m_kEventList(8), m_changeList(16), m_changePos(0)
{
    m_kq = kqueue();
    assert(m_kq != -1);
}

int KqueueDemultiplexer::registerEvent(handle_t handle, eventType et)
{
    if (m_changePos >= m_changeList.size())
        m_changeList.resize(m_changeList.size() * 2);
    struct kevent &event = m_changeList[m_changePos++];

    if (et & READ_EVENT)
        EV_SET(&event, handle, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (et * WRITE_EVENT)
        EV_SET(&event, handle, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
}
