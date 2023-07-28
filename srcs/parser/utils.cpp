#include "parserInternal.h"

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
        make_pair("PONG", parsePong),
    };
    const int size = sizeof(commands) / sizeof(commands[0]);

    for (int i = 0; i < size; i++)
        parsers[std::string(commands[i].first)] = commands[i].second;
}

std::string codeToString(eInvaldFormat code)
{
    switch (code)
    {
        case INVALID_PASSWORD:
            return MSG_INVALID_PASSWORD;
        case INVALID_NICKNAME:
            return MSG_INVALID_NICKNAME;
        case INVALID_USER:
            return MSG_INVALID_USER;
        case INVALID_HOSTNAME:
            return MSG_INVALID_HOSTNAME;
        case INVALID_MSG:
            return MSG_INVALID_MSG;
        default:
            return "Unknown error";
    }
}

std::string invalidFormatMsg(eInvaldFormat type, const std::string &msg)
{
    return codeToString(type) + ": " + msg;
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
    else // TODO: 추후 삭제
        kprintf_debug("not found CRLF\n");
}

bool isalnum(const std::string &str)
{
    for (size_t i = 0; i < str.length(); i++)
        if (!std::isalnum(str[i]))
            return false;
    return true;
}
}