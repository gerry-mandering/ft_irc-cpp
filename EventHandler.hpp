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

    handle_t getHandle() const { return m_handle; }
    virtual int handleRead() = 0;
    virtual int handleWrite();
    virtual int hadnleError();
};