#pragma once

#include "Request.hpp"
#include "parseException.hpp"
#include "requestConcrete.hpp"

typedef int handle_t;

class Request;

namespace Parser
{
enum eInvaldFormat
{
    INVALID_PASSWORD,
    INVALID_NICKNAME,
    INVALID_USER,
    INVALID_HOSTNAME,
    INVALID_HOSTNAME,
    INVALID_MSG,
};
} // namespace Parser

Request *parsePass(const std::string &tcpStream, handle_t socket);
Request *parseNick(const std::string &tcpStream, handle_t socket);
Request *parseUser(const std::string &tcpStream, handle_t socket);
Request *parseQuit(const std::string &tcpStream, handle_t socket);
Request *parseTopic(const std::string &tcpStream, handle_t socket);
Request *parseMode(const std::string &tcpStream, handle_t socket);
Request *parseJoin(const std::string &tcpStream, handle_t socket);
Request *parsePart(const std::string &tcpStream, handle_t socket);
Request *parseInvite(const std::string &tcpStream, handle_t socket);
Request *parseKick(const std::string &tcpStream, handle_t socket);
Request *parsePrivmsg(const std::string &tcpStream, handle_t socket);
Request *parsePing(const std::string &tcpStream, handle_t socket);
Request *parsePong(const std::string &tcpStream, handle_t socket);
bool isalnum(const std::string &str);
bool isalpha(const std::string &str);

typedef Request *(*parser_t)(const std::string &tcpStream);
