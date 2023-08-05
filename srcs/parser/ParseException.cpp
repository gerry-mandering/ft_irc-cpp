#include "ParseException.hpp"
#include "Client.hpp"
#include "parser_internal.h"
#include <iostream>
#include <sstream>

namespace Parser
{

SharedPtr< Client > findRegisteredClient(handle_t socket)
{
    SharedPtr< Client > client = ClientRepository::GetInstance()->FindBySocket(socket);

    if (!client)
    {
        LOG_INFO(__func__ << " Client not found");
        return (SharedPtr< Client >());
    }
    if (!client->HasRegistered())
    {
        LOG_INFO(" Client not registered");
        return (SharedPtr< Client >());
    }
    return client;
}

ParseException::ParseException(handle_t socket, const std::string &msg) throw() : m_socket(socket), m_msg(msg) {}

const char *ParseException::what() const throw()
{
    return m_msg.c_str();
}

ParseException::~ParseException() throw() {}

InvalidCommand::InvalidCommand(handle_t socket, const std::string &msg, const std::string &command) throw()
    : ParseException(socket, msg), m_command(command)
{
}

// irc.local 421 dah HI :Unknown command
void InvalidCommand::handleError() const throw()
{
    SharedPtr< Client > client = findRegisteredClient(m_socket);
    std::stringstream ss;

    if (client)
    {
        ss << "421 " << m_command << " :Unknown command\r\n";
        std::string responseMsg = ss.str();
        LOG_TRACE(" build response message: " << responseMsg);
        client->AddResponseToBuf(responseMsg);
    }
}

NotEnoughParams::NotEnoughParams(handle_t socket, const std::string &msg) throw() : ParseException(socket, msg) {}

void NotEnoughParams::handleError() const throw()
{
    SharedPtr< Client > client = findRegisteredClient(m_socket);

    if (client)
    {
        std::string responseMsg = "461 parameters :Not enough parameters\r\n";
        LOG_TRACE(" build response message: " << responseMsg);
        client->AddResponseToBuf(responseMsg);
    }
}

TooManyParams::TooManyParams(handle_t socket, const std::string &msg) throw() : ParseException(socket, msg) {}

void TooManyParams::handleError() const throw()
{
    SharedPtr< Client > client = findRegisteredClient(m_socket);

    if (client)
    {
        std::string responseMsg = "461 parameters :Too many parameters\r\n";
        LOG_TRACE(" build response message: " << responseMsg);
        client->AddResponseToBuf(responseMsg);
    }
}

InvalidFormat::InvalidFormat(handle_t socket, const std::string &msg, eInvaldFormat type) throw()
    : ParseException(socket, msg), type(type)
{
}

void InvalidFormat::handleError() const throw()
{
    SharedPtr< Client > client = findRegisteredClient(m_socket);

    if (client)
    {
        std::string responseMsg = "461 parameters :Invalid format\r\n";
        LOG_TRACE(" build response message: " << responseMsg);
        client->AddResponseToBuf(responseMsg);
    }
}

IgnoreExceptionCase::IgnoreExceptionCase(handle_t socket, const std::string &msg) throw() : ParseException(socket, msg)
{
}

void IgnoreExceptionCase::handleError() const throw()
{
    LOG_TRACE(" Ignore exception case: " << m_msg);
}
} // namespace Parser