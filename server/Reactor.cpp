#include "Reactor.hpp"

// KqueueDemultiplexer copy constructor로 멤버 이니셜라이저 구현
Reactor::Reactor() {
    mDemultiplexer = new KqueueDemultiplexer();
}

Reactor::~Reactor() {
    delete mDemultiplexer;
}

void Reactor::RegisterHandler(reactor::EventHandler *handler, reactor::eEventType eventType) {
    reactor::Socket socket = handler->GetSocket();

    mHandleMap[socket] = handler;
    mDemultiplexer->RequestEvent(socket, eventType);
}

void Reactor::RemoveHandler(reactor::EventHandler *handler, reactor::eEventType eventType) {
    reactor::Socket socket = handler->GetSocket();

    mHandleMap.erase(socket);
    mDemultiplexer->DeleteEvent(socket, eventType);
}

void Reactor::HandleEvents() {
    mDemultiplexer->WaitEvent(mHandleMap);
}
