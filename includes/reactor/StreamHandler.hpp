#pragma once

#include "EventHandler.hpp"
#include "Executor.hpp"
#include "ParseException.hpp"
#include "Request.hpp"
#include "Validator.hpp"
#include "parser_internal.h"
#include "shared_ptr.hpp"
#include <queue>
#include <string>

class Response
{
};

class StreamHandler : public EventHandler
{
    typedef ft::shared_ptr<Response> RequestPtr;

  private:
    // TODO: string 버퍼로 변경, respone 큐 추가
    // TODO: 민석2 responce 큐 clientRepo에 두기
    handle_t m_handle;
    std::string m_buf;
    std::queue<RequestPtr> m_responseQueue;
    const static int MAX_MSG_SIZE = 512;
    const static std::string CRLF;
    const static int CRLF_LEN = 2;

  public:
    StreamHandler(handle_t handle);
    ~StreamHandler();

    virtual handle_t getHandle(void) const;
    virtual int handleRead(void);
    virtual int handleWrite(void);
    virtual int handleError(void);

    bool hasRequest(std::string &requestStr);
};
