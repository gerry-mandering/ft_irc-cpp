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

void EnvManager::SetServerBootTime()
{
    std::time_t now = std::time(nullptr);
    std::tm *local_time = std::localtime(&now);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S %b %d %Y", local_time);

    mServerBootTime = std::string(buffer);
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

const std::string &EnvManager::GetServerBootTime() const
{
    return mServerBootTime;
}
