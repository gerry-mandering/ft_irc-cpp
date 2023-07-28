#include "debug.hpp"
#include "parserInternal.hpp"
#include <map>
#include <sstream>
#include <string>
#include <vector>

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
    else // TODO: 추후 삭제
        kprintf_debug("not found CRLF\n");
}

// parserRequest는 반드시 \r\n으로 끝나는 문자열을 인자로 받는다.
// handleread에서는 \r\n을 만나면 \r\n 앞부분까지를 추출해서 파서에 넘긴다.
// (단 이 경우 rfc 표준에 따라 512바이트를 초과하지 않도록 쌓아둔다)
Request *parseRequest(std::string &tcpStreams, handle_t socket)
{
    std::map<std::string, parser_t>::iterator it;
    std::stringstream ss(tcpStreams);
    std::string command;

    ss >> command;
    it = parsers.find(command);
    if (it != parsers.end())
        return (it->second)(tcpStreams);
    throw InvalidCommand(socket, std::string("Invalid command"));
}

Request *parsePass(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, password;

    if (!(ss >> command >> password))
        throw NotEnoughParams(socket, std::string("not enough params"));
    if (!isLastToken(password))
        throw TooManyParams(socket, std::string("too many params"));
    if (!isalnum(password))
        throw InvalidFormat(socket, std::string("Invalid naming password"), INVALID_PASSWORD);
    return (new PassRequest(socket, password));
}

Request *parseNick(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, nickname;

    if (!(ss >> command >> nickname))
        throw NotEnoughParams(socket, std::string("not enough params"));
    if (!isLastToken(nickname))
        throw TooManyParams(socket, std::string("too many params"));
    if (!isalpha(nickname.front()) || isalnum(nickname))
        throw InvalidFormat(socket, std::string("Invalid naming nickname"), INVALID_NICKNAME);
    return (new NickRequest(socket, nickname));
}

Request *parseUser(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, username, hostname, servername, realname;

    if (!(ss >> command >> username >> hostname >> servername >> realname))
        throw NotEnoughParams(socket, std::string("not enough params"));
    if (!isLastToken(realname))
        throw TooManyParams(socket, std::string("too many params"));
    if (!isalpha(username.front()) || !isalnum(username))
        throw InvalidFormat(socket, std::string("Invalid naming username"), INVALID_USER);
    if (!isalpha(hostname.front()) || !isalnum(hostname))
        throw InvalidFormat(socket, std::string("Invalid naming username"), INVALID_HOSTNAME);
    return (new UserRequest(socket, username, hostname, servername, realname));
}

Request *parseQuit(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, message;

    if (!(ss >> command >> message))
        throw NotEnoughParams(socket, std::string("not enough params"));
    if (isLastToken(command))
        return (new QuitRequest(socket, std::string())); // TODO: 디폴트 매개변수
    if (!isLastToken(message))
        throw TooManyParams(socket, std::string("too many params"));
    return (new QuitRequest(socket, message));
}

Request *parseTopic(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, topic;

    if (!(ss >> command >> channel >> topic))
        throw NotEnoughParams(socket, std::string("not enough params"));
    if (!isLastToken(topic))
        throw TooManyParams(socket, std::string("too many params"));
    return (new TopicRequest(socket, channel, topic));
}

// TODO: 추후 구현
Request *parseMode(const std::string &tcpStreams, handle_t socket)
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
    // return (new ModeRequest(socket, channel, mode));
    return (0);
}

Request *parseJoin(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, key;

    if (!(ss >> command >> channel))
        throw NotEnoughParams(socket, std::string("not enough params"));
    if (channel.front() != '#' || !isalnum(channel.substr(1)))
        throw InvalidFormat(socket, std::string("Invalid naming channel"), INVALID_HOSTNAME);
    if (isLastToken(channel))
        return (new JoinRequest(socket, channel, ""));
    ss >> key;
    if (!isLastToken(key))
        throw TooManyParams(socket, std::string("too many params"));
    return (new JoinRequest(socket, channel, key));
}

Request *parsePart(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, reason;

    if (!(ss >> command >> channel))
        throw NotEnoughParams(socket, std::string("not enough params"));
    if (isLastToken(channel))
        return (new PartRequest(socket, channel, ""));
    ss >> reason;
    if (!isLastToken(reason))
        throw TooManyParams(socket, std::string("too many params"));
    return (new PartRequest(socket, channel, reason));
}

Request *parsePrivmsg(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command;
    std::string receivers, msg, token;
    std::vector<std::string> receiverList;
    size_t pos, end;
    size_t start = 0;

    if (!(ss >> command >> receivers))
        throw NotEnoughParams(socket, std::string("not enough params"));
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
        throw TooManyParams(socket, std::string("too many params"));
    if (msg.front() != ':')
        throw InvalidFormat(socket, std::string("Invalid message"), INVALID_MSG);
    return (new PrivmsgRequest(socket, receiverList, msg));
}

}