#ifndef KQUEUE_DEMULTIPLEXER_H
#define KQUEUE_DEMULTIPLEXER_H

#include <iostream>
#include <sys/event.h>
#include <vector>
#include "../reactor_pattern/ReactorPattern.hpp"

class KqueueDemultiplexer : public reactor::IEventDemultiplexer {

public:
    KqueueDemultiplexer();
    ~KqueueDemultiplexer();

public:
    void WaitEvent(std::map<reactor::Socket, reactor::EventHandler *> &handlerMap);
    void RequestEvent(reactor::Socket socket, reactor::eEventType eventType);
    void DeleteEvent(reactor::Socket socket, reactor::eEventType EventType);

private:
    int mKqueueFd;
};

#endif