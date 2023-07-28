#include "ParseException.hpp"
#include "debug.h"
#include "parser_internal.h"

namespace Parser
{

extern std::map<std::string, parser_t> parsers;

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
        return (it->second)(tcpStreams, socket);
    throw InvalidCommand(socket, MSG_INVALID_CMD);
}

Request *parsePass(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, password;

    if (!(ss >> command >> password))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS);
    if (!isLastToken(password))
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS);
    if (!isalnum(password))
        throw InvalidFormat(socket, MSG_INVALID_PASSWORD, INVALID_PASSWORD);
    return (new PassRequest(socket, password));
}

Request *parseNick(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, nickname;

    if (!(ss >> command >> nickname))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS);
    if (!isLastToken(nickname))
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS);
    if (!std::isalpha(nickname.front()) || isalnum(nickname))
        throw InvalidFormat(socket, MSG_INVALID_NICKNAME, INVALID_NICKNAME);
    return (new NickRequest(socket, nickname));
}

Request *parseUser(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, username, hostname, servername, realname;

    if (!(ss >> command >> username >> hostname >> servername >> realname))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS);
    if (!isLastToken(realname))
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS);
    if (!std::isalpha(username.front()) || !isalnum(username))
        throw InvalidFormat(socket, MSG_INVALID_USER, INVALID_USER);
    if (!std::isalpha(hostname.front()) || !isalnum(hostname))
        throw InvalidFormat(socket, MSG_INVALID_HOSTNAME, INVALID_HOSTNAME);
    return (new UserRequest(socket, username, hostname, servername, realname));
}

Request *parseQuit(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, message;

    if (!(ss >> command >> message))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS);
    if (isLastToken(command))
        return (new QuitRequest(socket, "")); // TODO: 디폴트 매개변수
    if (!isLastToken(message))
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS);
    return (new QuitRequest(socket, message));
}

Request *parseTopic(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, topic;

    if (!(ss >> command >> channel >> topic))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS);
    if (!isLastToken(topic))
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS);
    return (new TopicRequest(socket, channel, topic));
}

// TODO: 추후 구현
Request *parseMode(const std::string &tcpStreams, handle_t socket)
{
    (void)tcpStreams;
    (void)socket;
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
    //     // throw MSG_TOO_MANY_PARAMS;
    // }
    // return (new ModeRequest(socket, channel, mode));
    return (0);
}

Request *parseJoin(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, key;

    if (!(ss >> command >> channel))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS);
    if (channel.front() != '#' || !isalnum(channel.substr(1)))
        throw InvalidFormat(socket, MSG_INVALID_CHANNEL, INVALID_HOSTNAME);
    if (isLastToken(channel))
        return (new JoinRequest(socket, channel, ""));
    ss >> key;
    if (!isLastToken(key))
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS);
    return (new JoinRequest(socket, channel, key));
}

Request *parsePart(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, reason;

    if (!(ss >> command >> channel))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS);
    if (isLastToken(channel))
        return (new PartRequest(socket, channel, ""));
    ss >> reason;
    if (!isLastToken(reason))
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS);
    return (new PartRequest(socket, channel, reason));
}

Request *parsePrivmsg(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command;
    std::string receivers, msg, token;
    std::vector<std::string> receiverList;
    size_t start = 0;
    size_t end;

    if (!(ss >> command >> receivers))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS);
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
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS);
    if (msg.front() != ':')
        throw InvalidFormat(socket, MSG_INVALID_MSG, INVALID_MSG);
    return (new PrivmsgRequest(socket, receiverList, msg));
}

Request *parseInvite(const std::string &tcpStreams, handle_t socket)
{
    (void)tcpStreams;
    (void)socket;
    return (0);
}

Request *parseKick(const std::string &tcpStreams, handle_t socket)
{
    (void)tcpStreams;
    (void)socket;
    return (0);
}

Request *parsePing(const std::string &tcpStreams, handle_t socket)
{
    (void)tcpStreams;
    (void)socket;
    return (0);
}

Request *parsePong(const std::string &tcpStreams, handle_t socket)
{
    (void)tcpStreams;
    (void)socket;
    return (0);
}

} // namespace Parser