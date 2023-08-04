#pragma once

#include "EventHandler.hpp"
#include "Executor.hpp"
#include "ParseException.hpp"
#include "Request.hpp"
#include "SharedPtr.hpp"
#include "Validator.hpp"
#include <queue>
#include <string>

class StreamHandler : public EventHandler
{
  private:
    handle_t m_handle;
    std::string m_readBuf;
    std::string m_writeBuf;

    const static int MAX_MSG_SIZE = 512;
    const static std::string CRLF;
    const static int CRLF_LEN = 2;
    const static int TMP_BUF_SIZE = 4096;

  public:
    StreamHandler(handle_t handle);
    ~StreamHandler();

    virtual handle_t getHandle(void) const;
    virtual int handleRead(void);
    virtual int handleWrite(void);
    virtual int handleDisconnect(void);

    bool hasRequest(std::string &requestStr);
    void addResponseToBuf(const std::string &response);
};
