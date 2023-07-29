#include "Logger.hpp"
#include "def.h"

Logger::Logger() : mProjectLogLevel(TRACE)
{
    fcntl(STDOUT_FILENO, F_SETFL, O_NONBLOCK);
    mSocket = STDOUT_FILENO;
}

void Logger::SetLogLevel(eLogLevel logLevel)
{
    mProjectLogLevel = logLevel;
}

// template <typename T>
// void Logger::Trace(const std::string &message, const T &object)
//{
//
// }

template <typename T>
void Logger::Debug(const std::string &message, const T &object)
{
    log(DEBUG, message, object);
}

template <typename T>
void Logger::Info(const std::string &message, const T &object)
{
    log(INFO, message, object);
}

template <typename T>
void Logger::Warn(const std::string &message, const T &object)
{
    log(WARN, message, object);
}

template <typename T>
void Logger::Error(const std::string &message, const T &object)
{
    log(ERROR, message, object);
}

handle_t Logger::getHandle() const
{
    return mSocket;
}

int Logger::handleWrite()
{
    std::string log = mLogQueue.front();
    mLogQueue.pop();

    // TODO write 하기
    std::cout << log << std::endl;

    if (mLogQueue.empty())
        g_reactor().unregisterEvent(this, WRITE_EVENT);

    return true;
}

template <typename T>
void Logger::log(eLogLevel logLevel, const std::string &message, const T &object)
{
    if (logLevel < mProjectLogLevel)
        return;

    const char *logStrings[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR"};
    int level = static_cast<int>(logLevel);

    std::stringstream logMessage;
    logMessage << logStrings[level] << " log = { " << message << " }";

    if (object)
        logMessage << " object = { " << object << " }";

    mLogQueue.push(logMessage.str());
    g_reactor().registerEvent(this, WRITE_EVENT);
}
