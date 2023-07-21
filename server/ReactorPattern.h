#ifndef REACTOR_PATTERN_H
#define REACTOR_PATTERN_H

#include <map>

namespace reactor {

    typedef int Socket;

    ///////////////////////////////////////////////////////////////////////////////////

    typedef enum eEventType {
        READ = 0x01,
        WRITE = 0x02,
        ERROR = 0x04,
        MASK = 0xff,
    } eEventType;

    ///////////////////////////////////////////////////////////////////////////////////

    class EventHandler {

    public:
        virtual Socket GetSocket() const = 0;
        virtual void HandleRead();
        virtual void HandleWrite();
        virtual void HandleError();

    protected:
        virtual ~EventHandler() {}

    };

    ///////////////////////////////////////////////////////////////////////////////////

    class IEventDemultiplexer {

    public:
        virtual void WaitEvent(std::map<Socket, EventHandler &> &handlerMap) = 0;
        virtual void RequestEvent(Socket socket, eEventType eventType) = 0;
        virtual void DeleteEvent(Socket socket) = 0;

    protected:
        virtual ~IEventDemultiplexer();

    };

    ///////////////////////////////////////////////////////////////////////////////////

    class IReactor {

    public:
        virtual void RegisterHandler(EventHandler &handler, eEventType eventType) = 0;
        virtual void RemoveHandler(EventHandler &handler) = 0;
        virtual void HandleEvents() = 0;

    protected:
        virtual ~IReactor() {}

    };

}

#endif