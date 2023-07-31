#pragma once

#include <exception>
#include <string>

namespace Wrapper
{

class SyscallException : public ::std::exception
{
  private:
    const ::std::string m_msg;

  public:
    SyscallException(const ::std::string &msg) throw();
    virtual const char *what() const throw();
    virtual ~SyscallException() throw() {} // Add this line
};
} // namespace Wrapper