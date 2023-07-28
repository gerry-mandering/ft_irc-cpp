#include "parseException.hpp"
#include <iostream>

namespace Parser
{

std::string codeTostring(eInvaldFormat code)
{
    switch (code)
    {
        case INVALID_PASSWORD:
            return "Invalid password";
        case INVALID_NICKNAME:
            return "Invalid nickname";
        case INVALID_USER:
            return "Invalid username";
        case INVALID_HOSTNAME:
            return "Invalid hostname";
        case INVALID_MSG:
            return "Invalid message";
        default:
            return "Unknown error";
    }
}

ParseException::ParseException(handle_t socket, const std::string &msg) : m_socket(socket), m_msg(msg) {}

const char *ParseException::what() const throw()
{
    return m_msg.c_str();
}

ParseException::~ParseException() throw() {}

InvalidCommand::InvalidCommand(handle_t socket, const std::string &msg) : ParseException(socket, msg) {}

void InvalidCommand::handleError() const throw()
{
    std::cout << what() << std::endl;
    return;
}

NotEnoughParams::NotEnoughParams(handle_t socket, const std::string &msg) : ParseException(socket, msg) {}

void NotEnoughParams::handleError() const throw()
{
    std::cout << what() << std::endl;
    return;
}

TooManyParams::TooManyParams(handle_t socket, const std::string &msg) : ParseException(socket, msg) {}

void TooManyParams::handleError() const throw()
{
    std::cout << what() << std::endl;
    return;
}

InvalidFormat::InvalidFormat(handle_t socket, const std::string &msg, eInvaldFormat type)
    : ParseException(socket, msg), type(type)
{
}

const char *InvalidFormat::what() const throw()
{
    std::string ret = codeTostring(type) + ": " + m_msg + "\n";
    return ret.c_str();
}

void InvalidFormat::handleError() const throw()
{
    std::cout << what() << std::endl;
    return;
}
} // namespace Parser