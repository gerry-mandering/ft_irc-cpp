#include "LoggingHandler.hpp"
#include "ParseException.hpp"
#include "debug.h"
#include "parser_internal.h"

namespace Parser
{

enum
{
    MAX_NICKNAME_LEN = 9,
    MAX_USERNAME_LEN = 9,
    MAX_HOSTNAME_LEN = 63,
    MAX_TOPIC_LEN = 50,
    MAX_CHANNEL_LEN = 200,
    // TODO: mode 명령어에서 키 설정할 때 키 길이 반드시 설정
    MAX_KEY_LEN = 15,
};

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
    LOG_TRACE("command: " << command);
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
    if (nickname.length() > MAX_NICKNAME_LEN || !std::isalpha(nickname.front()) || !isalnum(nickname))
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
    if (username.length() > MAX_USERNAME_LEN || !std::isalpha(username.front()) || !isalnum(username))
        throw InvalidFormat(socket, MSG_INVALID_USER + command, INVALID_USER);
    if (hostname.length() > MAX_HOSTNAME_LEN || !std::isalpha(hostname.front()) || !isalnum(hostname))
        throw InvalidFormat(socket, MSG_INVALID_HOSTNAME + command, INVALID_HOSTNAME);
    return (new UserRequest(socket, username, hostname, servername, realname));
}

Request *parseQuit(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, headOfLast, message;

    if (!(ss >> command))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    LOG_TRACE(__func__ << " command: " << command);
    if (!(ss >> headOfLast))
        // TODO: 디폴트 매개변수
        return (new QuitRequest(socket, ""));
    if (headOfLast.front() != ':')
        throw InvalidFormat(socket, MSG_INVALID_MSG + command, INVALID_MSG);
    message = tcpStreams.substr(tcpStreams.find(':', command.size() + 1));
    removeTrailingCRLF(message);
    LOG_TRACE(__func__ << " message: " << message);
    return (new QuitRequest(socket, message));
}

Request *parseTopic(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, topic, junk;

    if (!(ss >> command >> channel >> topic))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    if (topic.length() > MAX_TOPIC_LEN || !isalnum(topic))
        throw InvalidFormat(socket, MSG_INVALID_TOPIC + command, INVALID_TOPIC);
    if (ss >> junk)
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS + command);
    return (new TopicRequest(socket, channel, topic));
}

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
    if (channel.length() > MAX_CHANNEL_LEN || channel.front() != '#' || !isalnum(channel.substr(1)))
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
    removeTrailingCRLF(message);
    LOG_TRACE(__func__ << " command: " << command);
    LOG_TRACE(__func__ << " channel: " << channel);
    LOG_TRACE(__func__ << " message: " << message);
    return (new PartRequest(socket, channel, message));
}

static void commaToknizer(const std::string &targets, std::vector<std::string> &targetList)
{
    std::string token, lastToken;
    size_t start = 0;
    size_t end;

    end = targets.find(',');
    while (end != std::string::npos)
    {
        token = targets.substr(start, end - start);
        if (!token.empty())
            targetList.push_back(token);
        start = end + 1;
        end = targets.find(',', start);
    }
    lastToken = targets.substr(start);
    if (!lastToken.empty())
        targetList.push_back(lastToken);
}

Request *parsePrivmsg(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command;
    std::string targets, message, headOfLast, junk;
    std::vector<std::string> targetList;
    const size_t numSpace = 2;
    size_t preLastTokenLen;

    if (!(ss >> command >> targets))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    LOG_TRACE(__func__ << " command: " << command);
    LOG_TRACE(__func__ << " targets: " << targets);

    // TODO: "a,b,c," / "a, b,," / "a, b, c, ," / ",, ,"
    // TODO: 토큰에 :같은 특수문자가 있더라도 아무 상관없다. 어차피 검색해도 못찾을테니
    // ,가 연속으로 나오면 에러, / , 뒤에 아무것도 없으면 에러
    commaToknizer(targets, targetList);
    LOG_TRACE(__func__ << " targetList size: " << targetList.size());
    LOG_TRACE(targetList);
    if (!(ss >> headOfLast))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    if (headOfLast.front() != ':')
        throw InvalidFormat(socket, MSG_INVALID_MSG + command, INVALID_MSG);
    preLastTokenLen = command.size() + targets.size() + numSpace;
    message = tcpStreams.substr(tcpStreams.find(headOfLast, preLastTokenLen));
    removeTrailingCRLF(message);
    LOG_TRACE(__func__ << " message: " << message);
    return (new PrivmsgRequest(socket, targetList, message));
}

Request *parseKick(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, targets, headOfLast, message, junk;
    std::vector<std::string> targetList;
    const size_t numSpace = 2;
    size_t preLastTokenLen;

    if (!(ss >> command >> channel >> targets))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    LOG_TRACE(__func__ << " command: " << command);
    LOG_TRACE(__func__ << " channel: " << channel);
    commaToknizer(targets, targetList);
    LOG_TRACE(__func__ << " targetList size: " << targetList.size());
    LOG_TRACE(targetList);
    if (!(ss >> headOfLast))
        return (new KickRequest(socket, channel, targetList, ""));
    if (headOfLast.front() != ':')
        throw InvalidFormat(socket, MSG_INVALID_MSG + command, INVALID_MSG);
    preLastTokenLen = command.size() + channel.size() + targets.size() + numSpace;
    message = tcpStreams.substr(tcpStreams.find(headOfLast, preLastTokenLen));
    removeTrailingCRLF(message);
    LOG_TRACE(__func__ << " message: " << message);
    return (new KickRequest(socket, channel, targetList, message));
}

Request *parseInvite(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, nickname, channel, junk;

    if (!(ss >> command >> nickname >> channel))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    if (ss >> junk)
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS + command);
    return (new InviteRequest(socket, nickname, channel));
}

Request *parsePing(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, servername, junk;

    if (!(ss >> command >> servername))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    if (hasMetaChar(servername))
        throw InvalidFormat(socket, MSG_INVALID_SERVERNAME + command, INVALID_MSG);
    if (ss >> junk)
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS + command);
    return (new PingRequest(socket, servername));
}

} // namespace Parser