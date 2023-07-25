#ifndef REACTOR_PATTERN_H
#define REACTOR_PATTERN_H

#include <map>

namespace reactor {

    typedef int Socket;

    ///////////////////////////////////////////////////////////////////////////////////

    typedef enum eEventType {
        EVENT_READ = 0x01,
        EVENT_WRITE = 0x02,
        EVENT_ERROR = 0x04,
        EVENT_MASK = 0xff,
    } eEventType;

    ///////////////////////////////////////////////////////////////////////////////////

    class EventHandler {

    public:
        virtual ~EventHandler() {}

        virtual Socket GetSocket() const = 0;
        virtual void HandleRead() {}
        virtual void HandleWrite() {}
        virtual void HandleError() {}

    };

    ///////////////////////////////////////////////////////////////////////////////////

    class IEventDemultiplexer {

    public:
        virtual ~IEventDemultiplexer() {}

        virtual void WaitEvent(std::map<Socket, EventHandler *> &handlerMap) = 0;
        virtual void RequestEvent(Socket socket, eEventType eventType) = 0;
        virtual void DeleteEvent(Socket socket, eEventType eventType) = 0;

    };

    ///////////////////////////////////////////////////////////////////////////////////

    class IReactor {

    public:
        virtual ~IReactor() {}

        virtual void RegisterHandler(EventHandler *handler, eEventType eventType) = 0;
        virtual void RemoveHandler(EventHandler *handler, eEventType eventType) = 0;
        virtual void HandleEvents() = 0;

    };

}

#endif