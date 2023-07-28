#pragma once

#include "Request.hpp"
#include "debug.h"
#include "requestConcrete.hpp"
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace Parser
{

typedef int handle_t;
typedef Request *(*parser_t)(const std::string &tcpStream, handle_t socket);

typedef enum eInvaldFormat
{
    INVALID_PASSWORD,
    INVALID_NICKNAME,
    INVALID_USER,
    INVALID_HOSTNAME,
    INVALID_MSG,
} eInvaldFormat;

#define MSG_INVALID_CMD "Invalid command"
#define MSG_NOT_ENOUGH_PARAMS "Not enough parameters"
#define MSG_TOO_MANY_PARAMS "Too many parameters"
#define MSG_INVALID_PASSWORD "Invalid naming password"
#define MSG_INVALID_NICKNAME "Invalid naming nickname"
#define MSG_INVALID_USER "Invalid naming username"
#define MSG_INVALID_HOSTNAME "Invalid naming hostname"
#define MSG_INVALID_CHANNEL "Invalid naming channel"
#define MSG_INVALID_MSG "Invalid message"

std::string codeToString(eInvaldFormat code);
std::string invalidFormatMsg(eInvaldFormat type, const std::string &msg);
void removeTrailingCRLF(std::string &str);
bool isLastToken(const std::string &token);
bool isalnum(const std::string &str);
// TODO: isalph 필요성 여부
// bool isalpha(const std::string &str);

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

} // namespace Parser