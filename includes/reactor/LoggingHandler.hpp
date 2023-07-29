#pragma once

#include "EventHandler.hpp"
#include "Reactor.hpp"
#include "SingletonTemplate.hpp"
#include <queue>
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

#if LOGGING && (LOG_LEVEL >= LEVEL_TRACE)
#define LOG_TRACE(log) LoggingHandler::GetInstance()->pushLog(LEVEL_TRACE, log)
#else
#define LOG_TRACE(log)
#endif

#if LOGGING && (LOG_LEVEL >= LEVEL_DEBUG)
#define LOG_DEBUG(log) LoggingHandler::GetInstance()->pushLog(LEVEL_DEBUG, log)
#else
#define LOG_DEBUG(log)
#endif

#if LOGGING && (LOG_LEVEL >= LEVEL_INFO)
#define LOG_INFO(log) LoggingHandler::GetInstance()->pushLog(LEVEL_INFO, log)
#else
#define LOG_INFO(log)
#endif

#if LOGGING && (LOG_LEVEL >= LEVEL_WARN)
#define LOG_WARN(log) LoggingHandler::GetInstance()->pushLog(LEVEL_WARN, log)
#else
#define LOG_WARN(log)
#endif

#if (LOGGING && (LOG_LEVEL >= LEVEL_ERROR))
#define LOG_ERROR(log) LoggingHandler::GetInstance()->pushLog(LEVEL_ERROR, log)
#else
#define LOG_ERROR(log)
#endif

typedef std::pair<int, std::string> log_t;

class LoggingHandler : public EventHandler, public TSingleton<LoggingHandler>
{
  private:
    handle_t m_handle;
    std::queue<log_t> m_bufQueue;

  public:
    LoggingHandler();
    ~LoggingHandler();

    virtual handle_t getHandle(void) const;
    virtual int handleWrite(void);
    virtual int handleError(void);

    void pushLog(int level, const std::string &log);
    static std::string buildPrefix(int level);
    static std::string logLevelToString(int level);
};
