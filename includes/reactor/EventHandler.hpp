#pragma once

#include <string>

typedef enum eEventType
{
    READ_EVENT = 0x01,
    WRITE_EVENT = 0x02,
} eEventType;

typedef int handle_t;

class EventHandler
{
  protected:
    EventHandler() {}

  public:
    virtual ~EventHandler() {}

    virtual handle_t getHandle(void) const = 0;
    virtual int handleRead(void) { return 0; }
    virtual int handleWrite(void) { return 0; }
    // TODO: Error 처리 구현
    virtual int handleError(void) { return 0; }
    inline virtual void addResponseToBuf(const std::string &response) {}
};