#include "ParseException.hpp"
#include "Client.hpp"
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

// irc.local 421 dah HI :Unknown command
void InvalidCommand::handleError() const throw()
{
    Client *client = ClientRepository::GetInstance()->FindBySocket(m_socket);

    if (!client)
    {
        LOG_INFO(__func__ << " Client not found");
        return;
    }
    std::string nickname = "dah";
    std::string response = "421 " + std::string(" ") + nickname + " :this is my msg\r\n";
    LOG_TRACE(__func__ << " build Error response " << response);
    client->AddResponseToBuf(response);
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

modeException::modeException(handle_t socket, const std::string &msg) throw() : ParseException(socket, msg) {}

void modeException::handleError() const throw()
{
    std::cerr << what() << std::endl;
    return;
}
} // namespace Parser