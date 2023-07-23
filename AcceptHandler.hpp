#pragma once

#include "EventHandler.hpp"
#include <string>

class AcceptHandler : public EventHandler
{
  private:
    int m_port;
    std::string m_password;

  public:
    AcceptHandler(int port, std::string password);
    ~AcceptHandler();

    virtual int handleRead(void);
    virtual int handleWrite(void);
    virtual int handleError(void);
    bool init(void);
};