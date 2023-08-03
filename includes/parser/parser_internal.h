#pragma once

#include "Request.hpp"
#include "RequestConcrete.hpp"
#include "debug.h"
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace Parser
{

typedef int handle_t;
typedef Request *(*parser_t)(const std::string &tcpStream, handle_t socket);

enum
{
    MAX_NICKNAME_LEN = 9,
    MAX_USERNAME_LEN = 9,
    MAX_HOSTNAME_LEN = 63,
    MAX_TOPIC_LEN = 50,
    MAX_CHANNEL_LEN = 200,
    MAX_KEY_LEN = 15,
};

typedef enum eInvaldFormat
{
    INVALID_PASSWORD,
    INVALID_NICKNAME,
    INVALID_USER,
    INVALID_HOSTNAME,
    INVALID_SERVERNAME,
    INVALID_CHANNEL,
    INVALID_TOPIC,
    INVALID_KEY,
    INVALID_MSG,
    INVALID_MODE_OPTION,
    INVALID_MODE_ARGUMENT,
    // INVALID_MODE_LIMIT,
    // INVALID_MODE_USER
} eInvaldFormat;

#define MSG_INVALID_CMD "Invalid command"
#define MSG_NOT_ENOUGH_PARAMS "Not enough parameters"
#define MSG_TOO_MANY_PARAMS "Too many parameters"
#define MSG_INVALID_MSG "Invalid message"
#define MSG_MODE_EXCEPTION "Exception case: it won't be handled"

#define MSG_INVALID_PASSWORD "Invalid naming password"
#define MSG_INVALID_NICKNAME "Invalid naming nickname"
#define MSG_INVALID_USER "Invalid naming username"
#define MSG_INVALID_HOSTNAME "Invalid naming hostname"
#define MSG_INVALID_CHANNEL "Invalid naming channel"
#define MSG_INVALID_TOPIC "Invalid naming topic"
#define MSG_INVALID_KEY "Invalid naming key"
#define MSG_INVALID_SERVERNAME "Invalid naming servername"
#define MSG_INVALID_MODE_OPTION "Invalid mode option"
#define MSG_INVALID_MODE_ARGUMENT "Invalid mode argument"
// #define MSG_INVALID_MODE_LIMIT "Invalid mode argument"
// #define MSG_INVALID_MODE_USER "Invalid mode user"

std::string eTypeToString(Parser::eInvaldFormat code);
std::string invalidFormatMsg(Parser::eInvaldFormat type, const std::string &msg);
void removeTrailingCRLF(std::string &str);
bool isalnum(const std::string &str);
bool hasMetaChar(const std::string &str);
void commaToknizer(const std::string &targets, std::vector< std::string > &targetList);
bool invalidSign(const std::string &sign);
bool invalidModeType(const std::string &modeType);
bool notNeedOptionalToken(const std::string &sign, const std::string &modeType);
bool modeExceptionCase(const std::string &nickname, const std::string &modeToken);
bool invalidOptionalToken(const std::string &modeType, const std::string &optionalToken);

Request *parseRequest(const std::string &tcpStream, handle_t socket);
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

} // namespace Parser