#include "ParseException.hpp"
#include "parser_internal.h"
#include <iostream>

namespace Parser
{

ParseException::ParseException(handle_t socket, const std::string &msg) throw() : m_socket(socket), m_msg(msg) {}

const char *ParseException::what() const throw()
{
    return m_msg.c_str();
}

ParseException::~ParseException() throw() {}

InvalidCommand::InvalidCommand(handle_t socket, const std::string &msg) throw() : ParseException(socket, msg) {}

void InvalidCommand::handleError() const throw()
{
    std::cerr << what() << std::endl;
    return;
}

NotEnoughParams::NotEnoughParams(handle_t socket, const std::string &msg) throw() : ParseException(socket, msg) {}

void NotEnoughParams::handleError() const throw()
{
    std::cerr << what() << std::endl;
    return;
}

TooManyParams::TooManyParams(handle_t socket, const std::string &msg) throw() : ParseException(socket, msg) {}

void TooManyParams::handleError() const throw()
{
    std::cerr << what() << std::endl;
    return;
}

InvalidFormat::InvalidFormat(handle_t socket, const std::string &msg, eInvaldFormat type) throw()
    : ParseException(socket, msg), type(type)
{
}

void InvalidFormat::handleError() const throw()
{
    (void)type;
    std::cerr << what() << std::endl;
    return;
}
} // namespace Parser