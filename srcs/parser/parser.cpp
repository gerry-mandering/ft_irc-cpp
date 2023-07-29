#include "ParseException.hpp"
#include "debug.h"
#include "parser_internal.h"

// TODO: 마지막토큰에 딸려있는 \r 지우기
namespace Parser
{

extern std::map<std::string, parser_t> parsers;

// parserRequest는 반드시 \r\n으로 끝나는 문자열을 인자로 받는다.
// handleread에서는 \r\n을 만나면 \r\n 앞부분까지를 추출해서 파서에 넘긴다.
// (단 이 경우 rfc 표준에 따라 512바이트를 초과하지 않도록 쌓아둔다)
Request *parseRequest(const std::string &tcpStreams, handle_t socket)
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
    std::string command, password, junk;

    if (!(ss >> command >> password))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    if (ss >> junk)
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS + command);
    if (!isalnum(password))
        throw InvalidFormat(socket, MSG_INVALID_PASSWORD + command, INVALID_PASSWORD);
    return (new PassRequest(socket, password));
}

Request *parseNick(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, nickname, junk;

    if (!(ss >> command >> nickname))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    if (ss >> junk)
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS + command);
    if (!std::isalpha(nickname.front()) || !isalnum(nickname))
        throw InvalidFormat(socket, MSG_INVALID_NICKNAME + command, INVALID_NICKNAME);
    return (new NickRequest(socket, nickname));
}

Request *parseUser(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, username, hostname, servername, realname, junk;

    if (!(ss >> command >> username >> hostname >> servername >> realname))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    if (ss >> junk)
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS + command);
    if (!std::isalpha(username.front()) || !isalnum(username))
        throw InvalidFormat(socket, MSG_INVALID_USER + command, INVALID_USER);
    if (!std::isalpha(hostname.front()) || !isalnum(hostname))
        throw InvalidFormat(socket, MSG_INVALID_HOSTNAME + command, INVALID_HOSTNAME);
    return (new UserRequest(socket, username, hostname, servername, realname));
}

Request *parseQuit(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, headOfLast, message;

    if (!(ss >> command))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    if (!(ss >> headOfLast))
        return (new QuitRequest(socket, "")); // TODO: 디폴트 매개변수
    if (headOfLast.front() != ':')
        throw InvalidFormat(socket, MSG_INVALID_MSG + command, INVALID_MSG);
    message = tcpStreams.substr(tcpStreams.find(':', command.size() + 1));
    // std::cout << "Quit, message: " << message << std::endl;
    return (new QuitRequest(socket, message));
}

Request *parseTopic(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, topic, junk;

    if (!(ss >> command >> channel >> topic))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    if (!isalnum(topic))
        throw InvalidFormat(socket, MSG_INVALID_TOPIC + command, INVALID_TOPIC);
    if (ss >> junk)
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS + command);
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
    //     // throw MSG_TOO_MANY_PARAMS + command;
    // }
    // return (new ModeRequest(socket, channel, mode));
    return (0);
}

Request *parseJoin(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, key, junk;

    if (!(ss >> command >> channel))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    if (channel.front() != '#' || !isalnum(channel.substr(1)))
        throw InvalidFormat(socket, MSG_INVALID_CHANNEL + command, INVALID_CHANNEL);
    if (!(ss >> key))
        return (new JoinRequest(socket, channel, ""));
    // TODO: 입력 KEY에 이상한 문자가 있더라도 일단 validator에 보내는게 타당할듯
    // if (hasMetaChar(key))
    //     throw InvalidFormat(socket, MSG_INVALID_KEY + command, INVALID_KEY);
    if (ss >> junk)
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS + command);
    return (new JoinRequest(socket, channel, key));
}

Request *parsePart(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, headOfLast, message;

    // 채널명에 #이 없더라도 validator에 보낸후 이후 no such channel을 출력한다
    if (!(ss >> command >> channel))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    if (!(ss >> headOfLast))
        return (new PartRequest(socket, channel, ""));
    if (headOfLast.front() != ':')
        throw InvalidFormat(socket, MSG_INVALID_MSG + command, INVALID_MSG);
    message = tcpStreams.substr(tcpStreams.find(':', command.size() + channel.size() + 2));
    // std::cout << "Part, message: " << message << std::endl;
    return (new PartRequest(socket, channel, message));
}

Request *parsePrivmsg(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command;
    std::string receivers, message, token, headOfLast, junk;
    std::vector<std::string> receiverList;
    size_t start = 0;
    size_t end;

    if (!(ss >> command >> receivers))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
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
    if (!(ss >> headOfLast))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    if (headOfLast.front() != ':')
        throw InvalidFormat(socket, MSG_INVALID_MSG + command, INVALID_MSG);
    message = tcpStreams.substr(tcpStreams.find(headOfLast, command.size() + receivers.size() + 2));
    return (new PrivmsgRequest(socket, receiverList, message));
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