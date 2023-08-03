#include <map>
#include <string>
#include <utility>
#include <vector>

class Request;

Request *parserPass(const std::string &tcpStream);
Request *parserNick(const std::string &tcpStream);
Request *parserUser(const std::string &tcpStream);
Request *parserQuit(const std::string &tcpStream);
Request *parserTopic(const std::string &tcpStream);
Request *parserMode(const std::string &tcpStream);
Request *parserJoin(const std::string &tcpStream);
Request *parserPart(const std::string &tcpStream);
Request *parserInvite(const std::string &tcpStream);
Request *parserKick(const std::string &tcpStream);
Request *parserPrivmsg(const std::string &tcpStream);
Request *parserPing(const std::string &tcpStream);
Request *parserPong(const std::string &tcpStream);

typedef Request *(*parser_t)(const std::string &tcpStream);

std::map< std::string, parser_t > parsers;

void test_initParsers(void)
{
    std::pair< std::string, parser_t > commands[] = {
        make_pair("PASS", parserPass), make_pair("NICK", parserNick),       make_pair("USER", parserUser),
        make_pair("QUIT", parserQuit), make_pair("TOPIC", parserTopic),     make_pair("MODE", parserMode),
        make_pair("JOIN", parserJoin), make_pair("PART", parserPart),       make_pair("INVITE", parserInvite),
        make_pair("KICK", parserKick), make_pair("PRIVMSG", parserPrivmsg), make_pair("PING", parserPing),
        make_pair("PONG", parserPong),
    };
    const int size = sizeof(commands) / sizeof(commands[0]);
    for (int i = 0; i < size; i++)
        parsers[commands[i].first] = commands[i].second;
}

int main(void)
{
    test_initParsers();
    return (0);
}