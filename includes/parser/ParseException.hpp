#pragma once

#include "parser_internal.h"
#include <exception>
#include <string>

namespace Parser
{
class ParseException : public std::exception
{
  protected:
    handle_t m_socket;
    const std::string m_msg;

  public:
    ParseException(handle_t socket, const std::string &msg) throw();
    virtual ~ParseException() throw();
    virtual const char *what() const throw();
    virtual void handleError() const throw() = 0;
};

class InvalidCommand : public ParseException
{
  private:
    std::string m_command;

  public:
    InvalidCommand(handle_t socket, const std::string &msg, const std::string &command) throw();
    virtual void handleError() const throw();
    virtual ~InvalidCommand() throw() {}
};

class NotEnoughParams : public ParseException
{
  public:
    NotEnoughParams(handle_t socket, const std::string &msg) throw();
    virtual void handleError() const throw();
    virtual ~NotEnoughParams() throw() {}
};

class TooManyParams : public ParseException
{
  public:
    TooManyParams(handle_t socket, const std::string &msg) throw();
    virtual void handleError() const throw();
    virtual ~TooManyParams() throw() {}
};

class InvalidFormat : public ParseException
{
  private:
    eInvaldFormat type;

  public:
    InvalidFormat(handle_t socket, const std::string &msg, eInvaldFormat type) throw();
    virtual void handleError() const throw();
    virtual ~InvalidFormat() throw() {}
};

class IgnoreExceptionCase : public ParseException
{
  public:
    IgnoreExceptionCase(handle_t socket, const std::string &msg) throw();
    virtual void handleError() const throw();
    virtual ~IgnoreExceptionCase() throw() {}
};

} // namespace Parser