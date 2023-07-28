#include "debug.hpp"
#include "parserInternal.hpp"
#include <cstring>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace parser
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
        parsers[commands[i].first] = commands[i].second;
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
        kprintf_debug("not found CRLF\n");
}

// parserRequest는 반드시 \r\n으로 끝나는 문자열을 인자로 받는다.
// handleread에서는 \r\n을 만나면 \r\n 앞부분까지를 추출해서 파서에 넘긴다.
// (단 이 경우 rfc 표준에 따라 512바이트를 초과하지 않도록 쌓아둔다)
Request *parseRequest(std::string &tcpStreams)
{
    std::map<std::string, parser_t>::iterator it;
    std::stringstream ss(tcpStreams);
    std::string command;

    ss >> command;
    it = parsers.find(command);
    if (it != parsers.end())
        return (it->second)(tcpStreams);
    // thorw "command not found";
}

Request *parsePass(const std::string &tcpStreams)
{
    std::stringstream ss(tcpStreams);
    std::string command, password;

    // pos = tcpStreams.find(' ');
    // if (pos == std::string::npos)
    // {
    //     // throw "not enough params";
    // }

    // // 패스워드 검증 절차
    // // pos + 1은 out of range 절대 발생 안시킴
    // password = tcpStreams.substr(pos + 1);
    // if (password.size() < 1)
    // {
    //     // throw not enough params;
    // }
    // if (isalnum(password) == false)
    // {
    //     // throw "Invalid naming password";
    // }
    // ss << tcpStreams;
    if (!(ss >> command >> password))
    {
        // throw "not enough params";
    }
    if (!isLastToken(password))
    {
        // throw "too many params";
    }
    if (!isalnum(password))
    {
        // throw "Invalid naming password";
    }
    return (new RequestPass(command, password));
}

Request *parseNick(const std::string &tcpStreams)
{
    std::stringstream ss(tcpStreams);
    std::string command, nickname;

    // pos = tcpStreams.find(' ');
    // if (pos == std::string::npos)
    // {
    //     // throw "not enough params";
    // }
    // // 닉네임 검증 절차
    // nickname = tcpStreams.substr(pos + 1);
    // if (nickname.size() < 1)
    // {
    //     // throw not enough params;
    // }
    // if (isalpha(nickname.front()) == false || isalnum(nickname) == false)
    // {
    //     // throw "Invalid naming nickname";
    // }
    // ss << tcpStreams;
    // command = ss.str();
    if (!(ss >> command >> nickname))
    {
        // throw "not enough params";
    }
    if (!isLastToken(nickname))
    {
        // throw "too many params";
    }
    if (!isalpha(nickname.front()) || isalnum(nickname))
    {
        // throw "Invalid naming nickname";
    }
    return (new RequestNick(command, nickname));
}

Request *parseUser(const std::string &tcpStreams)
{
    std::stringstream ss(tcpStreams);
    std::string command, username, hostname, servername, realname;

    if (!(ss >> command >> username >> hostname >> servername >> realname))
    {
        // throw not enough prams;
    }
    if (!isLastToken(realname))
    {
        // throw "too many params";
    }
    if (!isalpha(username.front()) || !isalnum(username) || !isalpha(hostname.front()) || !isalnum(hostname))
    {
        // throw "Invalid naming username or hostname";
    }
    return (new RequestUser(command, username, hostname, servername, realname));
}

Request *parseQuit(const std::string &tcpStreams)
{
    std::stringstream ss(tcpStreams);
    std::string command, message;

    if (!(ss >> command >> message))
    {
        // throw not enough prams;
    }
    if (isLastToken(command))
    {
        return (new RequestQuit(command, "")); // TODO: 디폴트 매개변수
    }
    if (!isLastToken(message))
    {
        // throw "too many params";
    }
    return (new RequestQuit(command, message));
}

Request *parseTopic(const std::string &tcpStreams)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, topic;

    if (!(ss >> command >> channel >> topic))
    {
        // throw not enough prams;
    }
    if (!isLastToken(topic))
    {
        // throw "too many params";
    }
    return (new RequestTopic(command, channel, topic));
} // namespace parser

// TODO: 추후 구현
Request *parseMode(const std::string &tcpStreams)
{
    // std::stringstream ss(tcpStreams);
    // std::string command;
    // std::string channel;
    // std::string mode;

    // if (!(ss >> command >> channel >> mode))
    // {
    //     // throw not enough prams;
    // }
    // if (!isLastToken(mode))
    // {
    //     // throw "too many params";
    // }
    // return (new RequestMode(command, channel, mode));
}

Request *parseJoin(const std::string &tcpStreams)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, key;

    if (!(ss >> command >> channel))
    {
        // throw not enough prams;
    }
    if (channel.front() != '#' || !isalnum(channel.substr(1)))
    {
        // throw "Invalid channel name";
    }
    if (isLastToken(channel))
    {
        return (new RequestJoin(command, channel, ""));
    }
    ss >> key;
    if (!isLastToken(key))
    {
        // throw "too many params";
    }
    // TODO: requestJoin에 keyflag 추가?
    return (new RequestJoin(command, channel, key));
}

Request *parsePart(const std::string &tcpStreams)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel;

    if (!(ss >> command >> channel))
    {
        // throw not enough prams;
    }
    if (!isLastToken(channel))
    {
        // throw "too many params";
    }
    return (new RequestPart(command, channel));
}

Request *parsePrivmsg(const std::string &tcpStreams)
{
    std::stringstream ss(tcpStreams);
    std::string command;
    std::string receivers, msg, token;
    std::vector<std::string> receiverList;
    size_t pos, end;
    size_t start = 0;

    if (!(ss >> command >> receivers))
    {
        // throw not enough prams;
    }

    end = receivers.find(',');
    // TODO: "a,b,c," / "a, b,," / "a, b, c, ," / ",, ,"
    // ,가 연속으로 나오면 에러, / , 뒤에 아무것도 없으면 에러
    while (end != std::string::npos)
    {
        token = receivers.substr(start, end - start);
        receiverList.push_back(token);
        start = end + 1;
        end = receivers.find(',');
    }
    receiverList.push_back(receivers.substr(start));
    msg = tcpStreams.substr(tcpStreams.find(':') + 1);
    if (!isLastToken(msg))
    {
        throw "too many params";
    }
    if (msg.front() != ':')
    {
        throw "Invalid message";
    }
    return (new RequestPrivmsg(command, receiverList, msg));
}

} // namespace parser