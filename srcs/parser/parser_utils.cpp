#include "LoggingHandler.hpp"
#include "parser_internal.h"

namespace Parser
{
std::map<std::string, parser_t> parsers;

void initParsers(void)
{
    std::pair<std::string, parser_t> commands[] = {
        make_pair("PASS", parsePass), make_pair("NICK", parseNick),       make_pair("USER", parseUser),
        make_pair("QUIT", parseQuit), make_pair("TOPIC", parseTopic),     make_pair("MODE", parseMode),
        make_pair("JOIN", parseJoin), make_pair("PART", parsePart),       make_pair("INVITE", parseInvite),
        make_pair("KICK", parseKick), make_pair("PRIVMSG", parsePrivmsg), make_pair("PING", parsePing),
    };
    const int size = sizeof(commands) / sizeof(commands[0]);

    for (int i = 0; i < size; i++)
        parsers[commands[i].first] = commands[i].second;
}

std::string eTypeToString(eInvaldFormat type)
{
    switch (type)
    {
        case INVALID_PASSWORD:
            return MSG_INVALID_PASSWORD;
        case INVALID_NICKNAME:
            return MSG_INVALID_NICKNAME;
        case INVALID_USER:
            return MSG_INVALID_USER;
        case INVALID_HOSTNAME:
            return MSG_INVALID_HOSTNAME;
        case INVALID_CHANNEL:
            return MSG_INVALID_CHANNEL;
        case INVALID_TOPIC:
            return MSG_INVALID_TOPIC;
        case INVALID_KEY:
            return MSG_INVALID_KEY;
        case INVALID_MSG:
            return MSG_INVALID_MSG;
        default:
            return "Unknown error";
    }
}

std::string invalidFormatMsg(eInvaldFormat type, const std::string &msg)
{
    return eTypeToString(type) + ": " + msg;
}

// parse로 문자열이 넘길 때 항상 문자열 마지막에 \r\n이 존재한다.
bool isLastToken(const std::string &token)
{
    return (token.find("\r\n") != std::string::npos);
}

void removeTrailingCRLF(std::string &str)
{
    size_t pos;

    pos = str.find("\r\n");
    if (pos != std::string::npos)
        str.erase(pos);
    else
        LOG_WARN("not found CRLF");
}

void removeTrailingCR(std::string &str)
{
    size_t pos;

    pos = str.find('\r');
    if (pos != std::string::npos)
        str.erase(pos);
    else
        LOG_WARN("not found CR");
}

bool isalnum(const std::string &str)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        if (!std::isalnum(str[i]))
            return false;
    }
    return true;
}

bool hasMetaChar(const std::string &str)
{
    if (str.find('#') != std::string::npos || str.find(':') != std::string::npos || str.find(',') != std::string::npos)
        return true;
    return false;
}
} // namespace Parser