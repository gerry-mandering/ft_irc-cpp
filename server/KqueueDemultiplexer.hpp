#ifndef KQUEUE_DEMULTIPLEXER_H
#define KQUEUE_DEMULTIPLEXER_H

#include "ReactorPattern.h"

class KqueueDemultiplexer : public reactor::IEventDemultiplexer {

public:
    KqueueDemultiplexer();
    ~KqueueDemultiplexer();
    KqueueDemultiplexer(const KqueueDemultiplexer &other);
    KqueueDemultiplexer &operator=(const KqueueDemultiplexer &other);

public:
    void WaitEvent(std::map<reactor::Socket, reactor::EventHandler &> &handlerMap);
    void RequestEvent(Socket socket, eEventType eventType);
    void DeleteEvent(Socket socket);

private:

};

#endif