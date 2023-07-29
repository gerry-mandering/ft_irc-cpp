#pragma once

#include "EventHandler.hpp"
#include "SingletonTemplate.hpp"
#include <fcntl.h>
#include <iostream>
#include <queue>
#include <sstream>

#define STDOUT_FILENO 1

typedef enum eLogLevel
{
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR
} eLogLevel;

class Logger : public TSingleton<Logger>, public EventHandler
{
  public:
    Logger();
    ~Logger() {}

    void SetLogLevel(eLogLevel level);

    template <typename T>
    void Trace(const std::string &message, const T &object = T())
    {
        log(TRACE, message, object);
    }

    template <typename T>
    void Debug(const std::string &message, const T &object = T());

    template <typename T>
    void Info(const std::string &message, const T &object = T());

    template <typename T>
    void Warn(const std::string &message, const T &object = T());

    template <typename T>
    void Error(const std::string &message, const T &object = T());

    handle_t getHandle() const;
    int handleWrite();

  private:
    template <typename T>
    void log(eLogLevel logLevel, const std::string &message, const T &object);

    eLogLevel mProjectLogLevel;

    handle_t mSocket;
    std::queue<std::string> mLogQueue;
};
