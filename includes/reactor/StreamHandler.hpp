#pragma once

#include "EventHandler.hpp"
// #include "Request.hpp"
// #include "Executor.hpp"
// #include "Validator.hpp"
#include <string>

class StreamHandler : public EventHandler
{
  private:
    // TODO: string 버퍼로 변경, respone 큐 추가
    // TODO: 민석2 responce 큐 clientRepo에 두기
    handle_t m_handle;
    std::string m_buf;

  public:
    StreamHandler(handle_t handle);
    ~StreamHandler();

    virtual handle_t getHandle(void) const;
    virtual int handleRead(void);
    virtual int handleWrite(void);
    virtual int handleError(void);
};
