#include "LoggingHandler.hpp"
#include "color.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unistd.h>

#include <iostream>

// TODO: fcntl 추가, 로거는 생성자에서 registerHandler를 스스로 호출해서 등록
LoggingHandler::LoggingHandler() : EventHandler(OFF_EVENT), m_handle(STDOUT_FILENO)
{
    // fcntl(m_handle, F_SETFL, O_NONBLOCK);
    // Reactor::Getinstance()->registerHandler(this, WRITE_EVENT);
}

LoggingHandler::~LoggingHandler() {}

handle_t LoggingHandler::getHandle(void) const
{
    return m_handle;
}

int LoggingHandler::handleWrite(void)
{
    // size_t len = m_writeBuf.length();
    // ssize_t nwrite;

    // nwrite = write(m_handle, m_writeBuf.c_str(), len);
    // if (nwrite < 0)
    // {
    //     perror("standard_output write failed");
    //     exit(EXIT_FAILURE);
    // }
    // if (nwrite == len)
    // {
    //     m_writeBuf.clear();
    //     // return (Reactor::Getinstance()->unregisterEvent(this, WRITE_EVENT));
    //     return 0;
    // }
    // m_writeBuf = m_writeBuf.substr(nwrite);
    // return 0;
}

// TODO: 로거 핸들 에러 처리
int LoggingHandler::handleDisconnect(void)
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
    const char *levelColor[] = {COLOR_RESET, COLOR_RESET, BWHT, BYEL, BRED};

    // Write hour and minute with leading zeroes to the string stream
    oss << levelColor[level] << std::setfill('0') << std::setw(2) << localTime->tm_hour << ":" << std::setfill('0')
        << std::setw(2) << localTime->tm_min << ":" << std::setfill('0') << std::setw(2) << localTime->tm_sec;
    std::string prefix = oss.str() + " [" + LoggingHandler::logLevelToString(level) + "] ";
    return prefix;
}

void LoggingHandler::addWriteBuf(const std::string &str)
{
    m_writeBuf += str;
    // TODO: 이후 출력 kqueue에 등록해서 처리
    std::cout << m_writeBuf;
    m_writeBuf.clear();
}

const std::string &LoggingHandler::getWriteBuf(void) const
{
    return m_writeBuf;
}