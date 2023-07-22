#include "KqueueDemultiplexer.hpp"

KqueueDemultiplexer::KqueueDemultiplexer() {
    mKqueueFd = kqueue();
}

KqueueDemultiplexer::~KqueueDemultiplexer() {}

KqueueDemultiplexer::KqueueDemultiplexer(const KqueueDemultiplexer &other) {}

KqueueDemultiplexer &KqueueDemultiplexer::operator=(const KqueueDemultiplexer &other) {}

void KqueueDemultiplexer::WaitEvent(std::map<reactor::Socket, reactor::EventHandler *> &handlerMap) {
    const int MAX_EVENT_SIZE = 10;
    std::vector<struct kevent> eventlist(MAX_EVENT_SIZE);   //C++ 98 아닐 수도?
    int newEventCount;

    newEventCount = kevent(mKqueueFd, NULL, 0, &eventlist[0], MAX_EVENT_SIZE, NULL);
    if (newEventCount == -1) {
        std::cerr << "Fail to bring kevent" << std::endl;
    }

    // EventHandler udata에 넣어놓는 방식도 가능
    for (int i = 0; i < newEventCount; i++) {
        struct kevent *currentEvent = &eventlist[i];

        if (currentEvent->flags & EV_ERROR)
            handlerMap[currentEvent->ident]->HandleError();
        else if (currentEvent->filter == EVFILT_READ)
            handlerMap[currentEvent->ident]->HandleRead();
        else if (currentEvent->filter == EVFILT_WRITE)
            handlerMap[currentEvent->ident]->HandleWrite();
    }
}

void KqueueDemultiplexer::RequestEvent(reactor::Socket socket, reactor::eEventType eventType) {
    struct kevent event;

    if (eventType == reactor::EVENT_READ)
        EV_SET(&event, socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    else if (eventType == reactor::EVENT_WRITE)
        EV_SET(&event, socket, EVFILT_WRITE, EV_ADD, 0, 0, NULL);

    std::vector<struct kevent> changelist;
    changelist.push_back(event);

    if (kevent(mKqueueFd, &changelist[0], 1, NULL, 0, NULL) == -1) {
        std::cerr << "Fail to request event" << std::endl;
    }

//    changelist.clear();
}

void KqueueDemultiplexer::DeleteEvent(reactor::Socket socket, reactor::eEventType eventType) {
    struct kevent event;

    if (eventType == reactor::EVENT_READ)
        EV_SET(&event, socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    else if (eventType == reactor::EVENT_WRITE)
        EV_SET(&event, socket, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);

    std::vector<struct kevent> changelist;
    changelist.push_back(event);

    if (kevent(mKqueueFd, &changelist[0], 1, NULL, 0, NULL) == -1) {
        std::cerr << "Fail to delete event" << std::endl;
    }

//    changelist.clear();
}