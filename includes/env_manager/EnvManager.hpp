#pragma once

#include "SingletonTemplate.hpp"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

class EnvManager : public TSingleton< EnvManager >
{
  public:
    void SetConnectionPassword(const std::string &connectionPassword);
    void SetPortNumber(const std::string &portNumber);
    void SetServerBootTime();

    const std::string &GetServerName() const;
    const std::string &GetPortNumber() const;
    const std::string &GetConnectionPassord() const;
    const std::string &GetServerBootTime() const;

  private:
    static const std::string mServerName;
    std::string mPortNumber;
    std::string mConnectionPassword;
    std::string mServerBootTime;
};