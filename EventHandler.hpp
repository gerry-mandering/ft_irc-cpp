#pragma once

typedef int eventType;

enum
{
    READ_EVENT = 0x01,
    WRITE_EVENT = 0x02,
};

typedef int handle_t;

class EventHandler
{
  private:
    EventHandler();

  protected:
    handle_t m_handle;

  public:
    EventHandler(handle_t hadle);
    virtual ~EventHandler();

    handle_t getHandle() const;
    virtual int handleRead(void) = 0;
    virtual int handleWrite(void) = 0;
    // TODO: Error 처리 구현
    virtual int handleError(void) = 0;
};