#pragma once

#include "parserInternal.h"
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
  public:
    InvalidCommand(handle_t socket, const std::string &msg) throw();
    virtual void handleError() const throw();
};

class NotEnoughParams : public ParseException
{
  public:
    NotEnoughParams(handle_t socket, const std::string &msg) throw();
    virtual void handleError() const throw();
};

class TooManyParams : public ParseException
{
  public:
    TooManyParams(handle_t socket, const std::string &msg) throw();
    virtual void handleError() const throw();
};

class InvalidFormat : public ParseException
{
  private:
    eInvaldFormat type;

  public:
    InvalidFormat(handle_t socket, const std::string &msg, eInvaldFormat type) throw();
    virtual void handleError() const throw();
};

} // namespace Parser