#pragma once

#include "EventHandler.hpp"
#include "Reactor.hpp"
#include "SingletonTemplate.hpp"
#include <queue>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#define LEVEL_OFF 0xffffffff
#define LEVEL_TRACE 0
#define LEVEL_DEBUG 1
#define LEVEL_INFO 2
#define LEVEL_WARN 3
#define LEVEL_ERROR 4

#ifndef LOG_LEVEL
#define LOG_LEVEL LEVEL_OFF
#endif

#define LOGGER_REGISTER_EVENT Reactor::GetInstance()->registerEvent(LoggingHandler::GetInstance(), WRITE_EVENT);

#define LOG_TRACE(log)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (LOG_LEVEL <= LEVEL_TRACE)                                                                                  \
        {                                                                                                              \
            *(LoggingHandler::GetInstance()) << LoggingHandler::buildPrefix(LEVEL_TRACE) << log << "\n";               \
            if (LoggingHandler::GetInstance()->getEventFlag() == OFF_EVENT)                                            \
                LOGGER_REGISTER_EVENT                                                                                  \
        }                                                                                                              \
    } while (0);

#define LOG_DEBUG(log)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (LOG_LEVEL <= LEVEL_DEBUG)                                                                                  \
        {                                                                                                              \
            *(LoggingHandler::GetInstance()) << LoggingHandler::buildPrefix(LEVEL_DEBUG) << log << "\n";               \
            if (LoggingHandler::GetInstance()->getEventFlag() == OFF_EVENT)                                            \
                LOGGER_REGISTER_EVENT                                                                                  \
        }                                                                                                              \
    } while (0);

#define LOG_INFO(log)                                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        if (LOG_LEVEL <= LEVEL_INFO)                                                                                   \
        {                                                                                                              \
            *(LoggingHandler::GetInstance()) << LoggingHandler::buildPrefix(LEVEL_INFO) << log << "\n";                \
            if (LoggingHandler::GetInstance()->getEventFlag() == OFF_EVENT)                                            \
                LOGGER_REGISTER_EVENT                                                                                  \
        }                                                                                                              \
    } while (0);

#define LOG_WARN(log)                                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        if (LOG_LEVEL <= LEVEL_WARN)                                                                                   \
        {                                                                                                              \
            *(LoggingHandler::GetInstance()) << LoggingHandler::buildPrefix(LEVEL_WARN) << log << "\n";                \
            if (LoggingHandler::GetInstance()->getEventFlag() == OFF_EVENT)                                            \
                LOGGER_REGISTER_EVENT                                                                                  \
        }                                                                                                              \
    } while (0);

#define LOG_ERROR(log)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (LOG_LEVEL <= LEVEL_ERROR)                                                                                  \
        {                                                                                                              \
            *(LoggingHandler::GetInstance()) << LoggingHandler::buildPrefix(LEVEL_ERROR) << log << "\n";               \
            if (LoggingHandler::GetInstance()->getEventFlag() == OFF_EVENT)                                            \
                LOGGER_REGISTER_EVENT                                                                                  \
        }                                                                                                              \
    } while (0);

#define LOG_TRACE_SUSPEND(log)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        if (LOG_LEVEL <= LEVEL_TRACE)                                                                                  \
            *(LoggingHandler::GetInstance()) << LoggingHandler::buildPrefix(LEVEL_TRACE) << log << "\n";               \
    } while (0);

#define LOG_DEBUG_SUSPEND(log)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        if (LOG_LEVEL <= LEVEL_DEBUG)                                                                                  \
            *(LoggingHandler::GetInstance()) << LoggingHandler::buildPrefix(LEVEL_DEBUG) << log << "\n";               \
    } while (0);

#define LOG_INFO_SUSPEND(log)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        if (LOG_LEVEL <= LEVEL_INFO)                                                                                   \
            *(LoggingHandler::GetInstance()) << LoggingHandler::buildPrefix(LEVEL_INFO) << log << "\n";                \
    } while (0);

#define LOG_WARN_SUSPEND(log)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        if (LOG_LEVEL <= LEVEL_WARN)                                                                                   \
            *(LoggingHandler::GetInstance()) << LoggingHandler::buildPrefix(LEVEL_WARN) << log << "\n";                \
    } while (0);

#define LOG_ERROR_SUSPEND(log)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        if (LOG_LEVEL <= LEVEL_ERROR)                                                                                  \
            *(LoggingHandler::GetInstance()) << LoggingHandler::buildPrefix(LEVEL_ERROR) << log << "\n";               \
    } while (0);

class LoggingHandler : public EventHandler, public TSingleton< LoggingHandler >
{
  private:
    handle_t m_handle;
    std::string m_writeBuf;

  public:
    LoggingHandler();
    ~LoggingHandler();

    virtual handle_t getHandle(void) const;
    virtual int handleWrite(void);
    virtual int handleDisconnect(void);

    static std::string buildPrefix(int level);
    static std::string logLevelToString(int level);
    void addWriteBuf(std::string const &str);
    const std::string &getWriteBuf(void) const;
};

template < typename T >
inline LoggingHandler &operator<<(LoggingHandler &logger, T content)
{
    std::stringstream ss;
    ss << content;
    if (ss.fail())
        return logger;
    logger.addWriteBuf(ss.str());
    return logger;
}

template < typename T >
std::string convertVectorToString(const std::vector< T > &vec)
{
    std::stringstream ss;
    typename std::vector< T >::const_iterator it = vec.begin();
    ss << "[ ";
    for (; it != vec.end(); it++)
    {
        if (it != vec.begin())
            ss << ", ";
        ss << *it;
    }
    ss << " ]";
    return ss.str();
}

template < typename T >
inline LoggingHandler &operator<<(LoggingHandler &logger, const std::vector< T > &vec)
{
    logger.addWriteBuf(convertVectorToString(vec));
    return (logger);
}
