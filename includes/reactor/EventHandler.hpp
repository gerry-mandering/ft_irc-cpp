#pragma once

#include <string>

typedef enum eEventType
{
    OFF_EVENT = 0x00,
    READ_EVENT = 0x01,
    WRITE_EVENT = 0x02,
} eEventType;

typedef int handle_t;

class EventHandler
{
  protected:
    eEventType m_eventFlag;
    EventHandler(eEventType type) : m_eventFlag(type) {}

  public:
    virtual ~EventHandler() {}

    virtual handle_t getHandle(void) const = 0;
    eEventType getEventFlag(void) const { return m_eventFlag; }
    void setEventFlag(eEventType flag) { m_eventFlag = flag; }

    virtual int handleRead(void) { return 0; }
    virtual int handleWrite(void) { return 0; }
    // TODO: Error 처리 구현
    virtual int handleDisconnect(void) { return 0; }
    inline virtual void addResponseToBuf(const std::string &response) {}
};