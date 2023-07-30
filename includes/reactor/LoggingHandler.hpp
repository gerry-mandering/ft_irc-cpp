#pragma once

#include "EventHandler.hpp"
#include "Reactor.hpp"
#include "SingletonTemplate.hpp"
#include <queue>
#include <sstream>
#include <string>
#include <utility>

#include <iostream>

#define LEVEL_OFF -1
#define LEVEL_TRACE 0
#define LEVEL_DEBUG 1
#define LEVEL_INFO 2
#define LEVEL_WARN 3
#define LEVEL_ERROR 4

#ifndef LOG_LEVEL
#define LOG_LEVEL LEVEL_OFF
#endif

// TODO: 로거 리액터에 등록
// #define REGISTER_EVENT g_reactor().registerEvent(LoggingHandler::GetInstance(), WRITE_EVENT);

#define LOG_TRACE(log)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (LOG_LEVEL <= LEVEL_TRACE)                                                                                  \
        {                                                                                                              \
            *(LoggingHandler::GetInstance()) << LoggingHandler::buildPrefix(LEVEL_TRACE) << log << "\n";               \
        }                                                                                                              \
    } while (0);

#define LOG_DEBUG(log)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (LOG_LEVEL <= LEVEL_DEBUG)                                                                                  \
        {                                                                                                              \
            *(LoggingHandler::GetInstance()) << LoggingHandler::buildPrefix(LEVEL_DEBUG) << log << "\n";               \
        }                                                                                                              \
    } while (0);

#define LOG_INFO(log)                                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        if (LOG_LEVEL <= LEVEL_INFO)                                                                                   \
        {                                                                                                              \
            *(LoggingHandler::GetInstance()) << LoggingHandler::buildPrefix(LEVEL_INFO) << log << "\n";                \
        }                                                                                                              \
    } while (0);

#define LOG_WARN(log)                                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        if (LOG_LEVEL <= LEVEL_WARN)                                                                                   \
        {                                                                                                              \
            *(LoggingHandler::GetInstance()) << LoggingHandler::buildPrefix(LEVEL_WARN) << log << "\n";                \
        }                                                                                                              \
    } while (0);

#define LOG_ERROR(log)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (LOG_LEVEL <= LEVEL_ERROR)                                                                                  \
        {                                                                                                              \
            *(LoggingHandler::GetInstance()) << LoggingHandler::buildPrefix(LEVEL_ERROR) << log << "\n";               \
        }                                                                                                              \
    } while (0);

class LoggingHandler : public EventHandler, public TSingleton<LoggingHandler>
{
  private:
    handle_t m_handle;
    std::string m_writeBuf;

  public:
    LoggingHandler();
    ~LoggingHandler();

    virtual handle_t getHandle(void) const;
    virtual int handleWrite(void);
    virtual int handleError(void);

    static std::string buildPrefix(int level);
    static std::string logLevelToString(int level);
    void addWriteBuf(std::string const &str);
    const std::string &getWriteBuf(void) const;
};

template <typename T>
inline LoggingHandler &operator<<(LoggingHandler &logger, T content)
{
    std::stringstream ss;
    ss << content;
    logger.addWriteBuf(ss.str());
    return (logger);
}
