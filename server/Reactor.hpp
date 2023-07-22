#ifndef REACTOR_H
#define REACTOR_H

#include "SingletonTemplate.hpp"
#include "ReactorPattern.h"
#include "KqueueDemultiplexer.hpp"

class Reactor : public TSingleton<Reactor>, public reactor::IReactor {

public:
    Reactor();
    ~Reactor();
    Reactor(const Reactor &other);
    Reactor &operator=(const Reactor &other);

public:
    void RegisterHandler(reactor::EventHandler *handler, reactor::eEventType eventType);
    void RemoveHandler(reactor::EventHandler *handler, reactor::eEventType eventType);
    void HandleEvents();

private:
    reactor::IEventDemultiplexer *mDemultiplexer;
    std::map<reactor::Socket, reactor::EventHandler *> mHandleMap;

};

#endif
