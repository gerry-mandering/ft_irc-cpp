#ifndef REACTOR_H
#define REACTOR_H

#include "SingletonTemplate.hpp"
#include "ReactorPattern.h"

class Reactor : public TSingleton<Reactor>, public reactor::IReactor {

public:
    void RegisterHandler(EventHandler &handler, eEventType eventType);
    void RemoveHandler(EventHandler &handler);
    void HandleEvents();

public:
    Reactor();
    ~Reactor();
    Reactor(const Reactor &other);
    Reactor &operator=(const Reactor &other);

private:
    EventDemultiplexer &mDemultiplexer;
    std::map<reactor::Socket, EventHandler &> mHandlerMap;

};

#endif
