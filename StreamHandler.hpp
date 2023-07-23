#pragma once

#include "EventHandler.hpp"
#include <string>

class StreamHandler : public EventHandler
{
  private:
    // TODO: string 버퍼로 변경, respone 큐 추가
    std::string m_buf;

  public:
    StreamHandler(handle_t handle);
    ~StreamHandler();

    virtual int handleRead(void);
    virtual int handleWrite(void);
    virtual int hadnleError(void);
};