#include "parserInternal.hpp"
#include <cstring>
#include <map>
#include <string>
#include <vector>

namespace parser
{

std::map<std::string, parser_t> parsers;

void initParsers(void)
{
    std::pair<std::string, parser_t> commands[] = {
        make_pair("PASS", parserPass),       make_pair("NICK", parserNick),
        make_pair("USER", parserUser),       make_pair("QUIT", parserQuit),
        make_pair("TOPIC", parserTopic),     make_pair("MODE", parserMode),
        make_pair("JOIN", parserJoin),       make_pair("PART", parserPart),
        make_pair("INVITE", parserInvite),   make_pair("KICK", parserKick),
        make_pair("PRIVMSG", parserPrivmsg), make_pair("PING", parserPing),
        make_pair("PONG", parserPong),
    };
    const int size = sizeof(commands) / sizeof(commands[0]);
    for (int i = 0; i < size; i++)
        parsers[commands[i].first] = commands[i].second;
}

Request *parseRequest(const std::string &tcpStreams)
{
    std::stringstream ss;
    std::map<std::string, parser_t>::iterator it;

    ss << tcpStreams;
    it = parsers.find(ss.str());
    if (it != parsers.end())
        return (it->second)(tcpStreams);
    // TODO: return null vs throw
    // 에러 세분화에는 throw가 가장 적절하다. throw로 채태
    // thorw "Not found parser";
}

Request *parsePass(const std::string &tcpStreams)
{
    std::size_t pos;
    std::stringstream ss;
    std::string command;
    std::string password;

    pos = tcpStream.find(' ');
    if (pos == std::string::npos)
    {
        // throw "not enough param";
    }
    // 패스워드 검증 절차
    // pos + 1은 out of range 절대 발생 안시킴
    password = tcpStream.substr(pos + 1);
    if (password.size() < 1)
    {
        // throw not enough param;
    }
    if (isalnum(password) == false)
    {
        // throw "Invalid password";
    }
    ss << tcpStream;
    command = ss.str();
    return (new RequestPass(command, password));
}

Request *parseNick(const std::string &tcpStreams)
{
    std::size_t pos;
    std::stringstream ss;
    std::string command;
    std::string nickname;

    pos = tcpStream.find(' ');
    if (pos == std::string::npos)
    {
        // throw "not enough param";
    }
    // 닉네임 검증 절차
    nickname = tcpStream.substr(pos + 1);
    if (nickname.size() < 1)
    {
        // throw not enough param;
    }
    if (isalnum(nickname) == false)
    {
        // throw "Invalid nickname";
    }
    ss << tcpStream;
    command = ss.str();
    return (new RequestNick(command, nickname));
}

} // namespace parser