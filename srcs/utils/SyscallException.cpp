#include "SyscallException.hpp"

namespace Wrapper
{
SyscallException::SyscallException(const std::string &msg) throw() : m_msg(msg) {}

const char *SyscallException::what() const throw()
{
    return m_msg.c_str();
}
} // namespace Wrapper