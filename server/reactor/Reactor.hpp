#ifndef REACTOR_H
#define REACTOR_H

#include "../singleton_template/SingletonTemplate.hpp"
#include "../reactor_pattern/ReactorPattern.hpp"
#include "../kqueue_demultiplexer/KqueueDemultiplexer.hpp"

class Reactor : public TSingleton<Reactor>, public reactor::IReactor {

public:
    Reactor();
    ~Reactor();

    void RegisterHandler(reactor::EventHandler *handler, reactor::eEventType eventType);
    void RemoveHandler(reactor::EventHandler *handler, reactor::eEventType eventType);
    void HandleEvents();

private:
    // 의존성 주입
    reactor::IEventDemultiplexer *mDemultiplexer;
    std::map<reactor::Socket, reactor::EventHandler *> mHandleMap;

};

#endif
