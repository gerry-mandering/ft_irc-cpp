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
    handle_t m_handle;
    int eventMast;

  public:
    EventHandler(handle_t handle);
    virtual ~EventHandler();

    handle_t getHandle() const;
    virtual int handleRead() = 0;
    virtual int handleWrite() = 0;
    virtual int hadnleError() = 0;
};