#include "EnvManager.hpp"

const std::string EnvManager::mServerName = "irc.pingpong";

void EnvManager::SetConnectionPassword(const std::string &connectionPassword)
{
    mConnectionPassword = connectionPassword;
}

void EnvManager::SetPortNumber(const std::string &portNumber)
{
    mPortNumber = portNumber;
}

const std::string &EnvManager::GetServerName() const
{
    return mServerName;
}

const std::string &EnvManager::GetPortNumber() const
{
    return mPortNumber;
}

const std::string &EnvManager::GetConnectionPassord() const
{
    return mConnectionPassword;
}
