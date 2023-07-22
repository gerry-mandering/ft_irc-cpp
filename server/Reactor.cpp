#include "Reactor.hpp"

Reactor::Reactor() {
    mDemultiplexer = new KqueueDemultiplexer();
}

//DeleteInstance 오버라이딩
Reactor::~Reactor() {
    delete mDemultiplexer;
}

Reactor::Reactor(const Reactor &other) {}

Reactor &Reactor::operator=(const Reactor &other) {}

void Reactor::RegisterHandler(reactor::EventHandler *handler, reactor::eEventType eventType) {
    reactor::Socket socket = handler->GetSocket();

    // 중복추가 로직 필요?
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
