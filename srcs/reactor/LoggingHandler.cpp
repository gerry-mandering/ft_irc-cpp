#include "LoggingHandler.hpp"
#include "def.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unistd.h>

#include <iostream>

// TODO: fcntl 추가
LoggingHandler::LoggingHandler() : m_handle(STDIN_FILENO) {}

LoggingHandler::~LoggingHandler() {}

handle_t LoggingHandler::getHandle(void) const
{
    return m_handle;
}

int LoggingHandler::handleWrite(void)
{
    size_t len = m_writeBuf.length();
    ssize_t w_bytes;

    w_bytes = write(m_handle, m_writeBuf.c_str(), len);
    if (w_bytes < 0)
    {
        // TODO: 추후 삭제
        perror("standard_output write failed");
        exit(EXIT_FAILURE);
    }
    if (w_bytes == len)
    {
        m_writeBuf.clear();
        // return (g_reactor().unregisterEvent(this, WRITE_EVENT));
        return 0;
    }
    m_writeBuf = m_writeBuf.substr(w_bytes);
    return 0;
}

// TODO: 로거 핸들 에러 처리
int LoggingHandler::handleError(void)
{
    return 0;
}

std::string LoggingHandler::logLevelToString(int level)
{
    switch (level)
    {
        case LEVEL_TRACE:
            return "TRACE";
        case LEVEL_DEBUG:
            return "DEBUG";
        case LEVEL_INFO:
            return "INFO ";
        case LEVEL_WARN:
            return "WARN ";
        case LEVEL_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

std::string LoggingHandler::buildPrefix(int level)
{
    // Get current time
    std::time_t t = std::time(nullptr);
    // Convert to local time
    std::tm *localTime = std::localtime(&t);
    // Create a string stream
    std::ostringstream oss;

    // Write hour and minute with leading zeroes to the string stream
    oss << std::setfill('0') << std::setw(2) << localTime->tm_hour << ":" << std::setfill('0') << std::setw(2)
        << localTime->tm_min << ":" << std::setfill('0') << std::setw(2) << localTime->tm_sec;
    std::string prefix = oss.str() + " [" + LoggingHandler::logLevelToString(level) + "] ";
    return prefix;
}

void LoggingHandler::addWriteBuf(const std::string &str)
{
    m_writeBuf += str;
}