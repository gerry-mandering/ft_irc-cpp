#include "LoggingHandler.hpp"
#include "Reactor.hpp"
#include "color.h"
#include <ctime>
#include <fcntl.h>
#include <iomanip>
#include <sstream>
#include <unistd.h>

#include <iostream>

LoggingHandler::LoggingHandler() : EventHandler(OFF_EVENT), m_handle(STDOUT_FILENO)
{
    fcntl(m_handle, F_SETFL, O_NONBLOCK);
}

LoggingHandler::~LoggingHandler() {}

handle_t LoggingHandler::getHandle(void) const
{
    return m_handle;
}

int LoggingHandler::handleWrite(void)
{
    ssize_t nwrite;

    nwrite = write(m_handle, m_writeBuf.c_str(), m_writeBuf.size());
    if (nwrite < 0)
        return CODE_OK;
    if ((size_t)nwrite < m_writeBuf.size())
    {
        m_writeBuf = m_writeBuf.substr(nwrite);
        return CODE_OK;
    }
    if ((size_t)nwrite == m_writeBuf.size())
    {
        m_writeBuf.clear();
        return (Reactor::GetInstance()->unregisterEvent(this, WRITE_EVENT));
    }
    return CODE_OK;
}

// TODO: 로거 핸들 에러 처리
int LoggingHandler::handleDisconnect(void)
{
    std::cerr << BRED << "LoggingHandler Disconnect\n";
    exit(EXIT_FAILURE);
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

// 반환형 string으로
std::string LoggingHandler::buildPrefix(int level)
{
    // Get current time
    std::time_t t = std::time(NULL);
    // Convert to local time
    std::tm *localTime = std::localtime(&t);
    // Create a string stream
    std::stringstream ss;
    const char *levelColor[] = {COLOR_RESET, COLOR_RESET, BWHT, BYEL, BRED};

    // Write hour and minute with leading zeroes to the string stream
    ss << levelColor[level] << std::setfill('0') << std::setw(2) << localTime->tm_hour << ":" << std::setfill('0')
       << std::setw(2) << localTime->tm_min << ":" << std::setfill('0') << std::setw(2) << localTime->tm_sec;
    std::string prefix = ss.str() + " [" + LoggingHandler::logLevelToString(level) + "] ";
    return prefix;
}

void LoggingHandler::addWriteBuf(const std::string &str)
{
    // std::cerr << "addWriteBuf: " << str << std::endl;
    m_writeBuf += str;
}

const std::string &LoggingHandler::getWriteBuf(void) const
{
    return m_writeBuf;
}