#pragma once

#include "SingletonTemplate.hpp"
#include <iostream>

class EnvManager : public TSingleton<EnvManager>
{
  public:
    void SetConnectionPassword(const std::string &connectionPassword);
    void SetPortNumber(const std::string &portNumber);

    const std::string &GetServerName() const;
    const std::string &GetPortNumber() const;
    const std::string &GetConnectionPassord() const;

  private:
    static const std::string mServerName;
    std::string mPortNumber;
    std::string mConnectionPassword;
};