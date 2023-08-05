#include "parser.h"
#include "LoggingHandler.hpp"
#include "ParseException.hpp"
#include "debug.h"
#include "parser_internal.h"

namespace Parser
{

extern std::map< std::string, parser_t > parsers;

// parserRequest는 반드시 \r\n으로 끝나는 문자열을 인자로 받는다.
// handleread에서는 \r\n을 만나면 \r\n 앞부분까지를 추출해서 파서에 넘긴다.
// (단 이 경우 rfc 표준에 따라 512바이트를 초과하지 않도록 쌓아둔다)
Request *parseRequest(const std::string &tcpStreams, handle_t socket)
{
    std::map< std::string, parser_t >::iterator it;
    std::stringstream ss(tcpStreams);
    std::string command;

    ss >> command;
    LOG_TRACE("parseRequest command: " << command);
    if (ShouldIgnoreCommand(tcpStreams) || command.empty())
        throw IgnoreExceptionCase(socket, "Command: " + command);
    it = parsers.find(command);
    if (it != parsers.end())
        return (it->second)(tcpStreams, socket);
    throw InvalidCommand(socket, MSG_INVALID_CMD, command);
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
    LOG_TRACE(" command: " << command);
    if (!(ss >> headOfLast))
        // TODO: 디폴트 매개변수
        return (new QuitRequest(socket, ""));
    if (headOfLast.front() != ':')
        throw InvalidFormat(socket, MSG_INVALID_MSG + command, INVALID_MSG);
    message = tcpStreams.substr(tcpStreams.find(':', command.size() + 1));
    removeTrailingCRLF(message);
    LOG_TRACE(" message: " << message);
    return (new QuitRequest(socket, message));
}

Request *parseTopic(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, headOfLast, topic;
    const size_t numSpace = 1;
    size_t preLastTokenLen;

    if (!(ss >> command >> channel >> headOfLast))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    LOG_TRACE(" command: " << command);
    LOG_TRACE(" channel: " << channel);
    if (headOfLast.front() != ':')
        throw InvalidFormat(socket, MSG_INVALID_TOPIC + command, INVALID_TOPIC);
    preLastTokenLen = command.size() + channel.size() + numSpace;
    topic = tcpStreams.substr(tcpStreams.find(headOfLast, preLastTokenLen));
    LOG_TRACE(" topic: " << topic);
    if (topic.length() > MAX_TOPIC_LEN || hasMetaChar(topic.substr(1)))
        throw InvalidFormat(socket, MSG_INVALID_TOPIC + command, INVALID_TOPIC);
    return (new TopicRequest(socket, channel, topic));
}

Request *parseMode(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, modeToken, sign, modeType, optionalToken, junk;

    if (!(ss >> command >> channel >> modeToken))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    sign = modeToken.front();
    modeType = modeToken.substr(1);
    if (invalidSign(sign) || invalidModeType(modeType))
        throw InvalidFormat(socket, MSG_INVALID_MODE_OPTION + command, INVALID_MODE_OPTION);
    LOG_TRACE(" sign: " << sign << " modeType: " << modeType);
    ss >> optionalToken;
    if (notNeedOptionalToken(sign, modeType))
    {
        if (!optionalToken.empty())
            throw TooManyParams(socket, MSG_TOO_MANY_PARAMS + command);
        return (new ModeRequest(socket, channel, sign, modeType, ""));
    }
    if (optionalToken.empty())
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    LOG_TRACE(" optionalToken: " << optionalToken)
    if (invalidOptionalToken(modeType, optionalToken))
        throw InvalidFormat(socket, MSG_INVALID_MODE_ARGUMENT + command, INVALID_MODE_ARGUMENT);
    if (ss >> junk)
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS + command);
    return (new ModeRequest(socket, channel, sign, modeType, optionalToken));
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
    LOG_TRACE(" command: " << command);
    LOG_TRACE(" channel: " << channel);
    LOG_TRACE(" message: " << message);
    return (new PartRequest(socket, channel, message));
}

Request *parsePrivmsg(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command;
    std::string targets, message, headOfLast, junk;
    std::vector< std::string > targetList;
    const size_t numSpace = 2;
    size_t preLastTokenLen;

    if (!(ss >> command >> targets))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    LOG_TRACE(" command: " << command);
    LOG_TRACE(" targets: " << targets);

    commaToknizer(targets, targetList);
    LOG_TRACE(" targetList size: " << targetList.size());
    LOG_TRACE(targetList);
    if (!(ss >> headOfLast))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    if (headOfLast.front() != ':')
        throw InvalidFormat(socket, MSG_INVALID_MSG + command, INVALID_MSG);
    preLastTokenLen = command.size() + targets.size() + numSpace;
    message = tcpStreams.substr(tcpStreams.find(headOfLast, preLastTokenLen));
    removeTrailingCRLF(message);
    LOG_TRACE(" message: " << message);
    return (new PrivmsgRequest(socket, targetList, message));
}

Request *parseKick(const std::string &tcpStreams, handle_t socket)
{
    std::stringstream ss(tcpStreams);
    std::string command, channel, targets, headOfLast, message, junk;
    std::vector< std::string > targetList;
    const size_t numSpace = 2;
    size_t preLastTokenLen;

    if (!(ss >> command >> channel >> targets))
        throw NotEnoughParams(socket, MSG_NOT_ENOUGH_PARAMS + command);
    LOG_TRACE(" command: " << command);
    LOG_TRACE(" channel: " << channel);
    commaToknizer(targets, targetList);
    LOG_TRACE(" targetList size: " << targetList.size());
    LOG_TRACE(targetList);
    if (!(ss >> headOfLast))
        return (new KickRequest(socket, channel, targetList, ""));
    if (headOfLast.front() != ':')
        throw InvalidFormat(socket, MSG_INVALID_MSG + command, INVALID_MSG);
    preLastTokenLen = command.size() + channel.size() + targets.size() + numSpace;
    message = tcpStreams.substr(tcpStreams.find(headOfLast, preLastTokenLen));
    removeTrailingCRLF(message);
    LOG_TRACE(" message: " << message);
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
        throw InvalidFormat(socket, MSG_INVALID_SERVERNAME + command, INVALID_SERVERNAME);
    if (ss >> junk)
        throw TooManyParams(socket, MSG_TOO_MANY_PARAMS + command);
    return (new PingRequest(socket, servername));
}

} // namespace Parser