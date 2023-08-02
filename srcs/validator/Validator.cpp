/*
 * PASS, NICK, USER, CAP 빼고는 다 Registered 안되어 있으면 NotRegisteredMsg
 *
 * <Validator 구현 완료 명령어>
 *
 * PASS
 * NICK
 * USER
 * TOPIC
 * PING
 * PART
 * PRIVMSG
 * QUIT
 * INVITE
 * KICK
 *
 * <Validator 미완료 명령어>
 * JOIN
 * MODE
 *
 * */

#include "Validator.hpp"

using namespace ft_validator;

namespace ft_validator
{
// 로그 찍기 위해 exp 분할
bool keyModeOK(Channel *channel, const std::string &key)
{
    if (!channel->IsKeyMode())
    {
        LOG_DEBUG("Joining channel is not in key mode");
        return true;
    }
    LOG_DEBUG("Joining channel is in key mode");
    return (channel->GetKey() == key);
    // return (!channel->IsKeyMode() || channel->GetKey() == key);
}

bool limitModeOK(Channel *channel)
{
    if (!channel->IsClientLimitMode())
    {
        LOG_DEBUG("Joining channel is not in limit mode");
        return true;
    }
    LOG_DEBUG("Joining channel is in limit mode");
    return (channel->GetClientLimit() > channel->GetClientCount());
    // return (!channel->IsClientLimitMode() || channel->GetClientLimit() > channel->GetClientCount());
}

bool notAlreadyInChan(Client *client, Channel *channel)
{
    if (!channel->CheckClientIsExist(client->GetNickName()))
    {
        LOG_DEBUG("Client is not in channel");
        return true;
    }
    LOG_DEBUG("Client is already in channel");
    return (false);
    // return (!channel->CheckClientIsExist(client->GetNickName()));
}
} // namespace ft_validator

bool Validator::Visit(CapRequest *capRequest) const
{
    return true;
}

bool Validator::Visit(InviteRequest *inviteRequest) const
{
    Client *client = inviteRequest->GetClient();
    const std::string &nickName = client->GetNickName();
    const std::string &channelName = inviteRequest->GetChannelName();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage = buildNotRegisteredMsg("INVITE", nickName);
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("InviteRequest Invalid - NotRegistered");

        return false;
    }

    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(channelName);

    // 채널이 없는 경우
    if (!channel)
    {
        std::string errorMessage;
        errorMessage = buildNoSuchChannelMsg(nickName, channelName);

        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("InviteRequest Invalid - NoSuchChannel");

        return false;
    }

    ClientRepository *clientRepository = ClientRepository::GetInstance();
    Client *targetClient = clientRepository->FindByNickName(nickName);

    // 해당 닉네임을 가진 유저가 없는 경우
    if (!targetClient)
    {
        std::string errorMessage;
        errorMessage = buildNoSuchNickMsg(nickName, inviteRequest->GetNickName());

        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("InviteRequest Invalid - NoSuchNick");

        return false;
    }

    // 요청을 한 클라이언트가 채널 들어가 있지 않는 경우
    if (!channel->CheckClientIsExist(nickName))
    {
        std::string errorMessage;
        errorMessage = buildNotOnChannelMsg(nickName, channelName);

        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("InviteRequest Invalid - NotOnChannel");

        return false;
    }

    // 이미 초대한 유저가 채널에 존재하는 경우
    if (channel->CheckClientIsExist(inviteRequest->GetNickName()))
    {
        std::string errorMessage;
        errorMessage = buildUserOnChannelMsg(nickName, inviteRequest->GetNickName(), inviteRequest->GetChannelName());

        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("InviteRequest Invalid - UserOnChannel");

        return false;
    }

    // 채널 operator가 아닌 경우
    if (channel->CheckClientIsOperator(nickName))
    {
        std::string errorMessage;
        errorMessage = buildNotChannelOperatorMsg(nickName, channelName);

        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("InviteRequest Invalid - NotChannelOperator");

        return false;
    }

    LOG_TRACE("InviteRequest Validated");

    return true;
}

// TODO InviteOnlyMode 일때 초대 받은 사람인지 검증하는 로직 추가
bool Validator::Visit(JoinRequest *joinRequest) const
{

    ChannelRepository *channelRepo = ChannelRepository::GetInstance();
    Channel *channel;
    Client *client = joinRequest->GetClient();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage = buildNotRegisteredMsg("JOIN", client->GetNickName());
        client->AddResponseToBuf(errorMessage);

        LOG_DEBUG("JoinRequest Invalid - NotRegistered");

        return false;
    }

    channel = channelRepo->FindByName(joinRequest->getChannelName());
    // 채널에 처음 입장할 때(즉 생성)
    if (channel == NULL)
    {
        LOG_DEBUG("JoinRequest is valid: Channel name is not exist");
        return true;
    }
    if (keyModeOK(channel, joinRequest->getKey()) && limitModeOK(channel) && notAlreadyInChan(client, channel))
    {
        LOG_DEBUG("JoinRequest is valid: pass all condition");
        return true;
    }
    // TODO: 메시지 형식 만들것, 세분화 할 필요 없다고 생각함
    // client->InsertResponse("에러코드 + 메시지");
    LOG_DEBUG("JoinRequest is not valid");

    return false;
}

bool Validator::Visit(KickRequest *kickRequest) const
{
    Client *client = kickRequest->GetClient();
    const std::string &nickName = client->GetNickName();
    const std::string &channelName = kickRequest->GetChannelName();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage = buildNotRegisteredMsg("KICK", nickName);
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("KickRequest Invalid - NotRegistered");

        return false;
    }

    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(channelName);

    // 채널이 없는 경우
    if (!channel)
    {
        std::string errorMessage;

        errorMessage = buildNoSuchChannelMsg(nickName, channelName);

        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("KickRequest Invalid - NoSuchChannel");

        return false;
    }

    ClientRepository *clientRepository = ClientRepository::GetInstance();

    std::vector<std::string> validatedTargets;
    std::vector<std::string> targets = kickRequest->GetTargets();
    std::vector<std::string>::iterator iter;

    for (iter = targets.begin(); iter != targets.end(); iter++)
    {
        Client *targetClient = clientRepository->FindByNickName(*iter);

        // 닉네임이 존재하지 않는 경우
        if (!targetClient)
        {
            std::string errorMessage;
            errorMessage = buildNoSuchNickMsg(nickName, *iter);

            client->AddResponseToBuf(errorMessage);

            LOG_TRACE("KickRequest Invalid - NoSuchNick");

            continue;
        }

        // 요청을 한 클라이언트가 채널 들어가 있지 않는 경우
        if (!channel->CheckClientIsExist(nickName))
        {
            std::string errorMessage;
            errorMessage = buildNotOnChannelMsg(nickName, channelName);

            client->AddResponseToBuf(errorMessage);

            LOG_TRACE("KickRequest Invalid - NotOnChannel");

            continue;
        }

        // target 클라이언트가 채널에 들어가 있지 않는 경우
        if (!channel->CheckClientIsExist(*iter))
        {
            std::string errorMessage;
            errorMessage = buildUserNotInChannelMsg(nickName, *iter, channelName);

            client->AddResponseToBuf(errorMessage);

            LOG_TRACE("KickRequest Invalid - UserNotOnChannel");

            continue;
        }

        // 채널 operator가 아닌 경우
        if (channel->CheckClientIsOperator(nickName))
        {
            std::string errorMessage;
            errorMessage = buildNotChannelOperatorMsg(nickName, channelName);

            client->AddResponseToBuf(errorMessage);

            LOG_TRACE("KickRequest Invalid - NotChannelOperator");

            return false;
        }

        validatedTargets.push_back(*iter);
    }

    if (validatedTargets.empty())
    {
        LOG_TRACE("KickRequest Invalid - ValidatedTargetsEmpty");
        return false;
    }
    else
    {
        targets = validatedTargets;

        LOG_TRACE("KickRequest Validated");
        return true;
    }
}

bool Validator::Visit(ModeRequest *modeRequest) const
{
    Client *client = modeRequest->GetClient();
    const std::string &nickName = client->GetNickName();
    const std::string &channelName = modeRequest->GetChannelName();
    const std::string &modeChar = modeRequest->GetModeChar();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage = buildNotRegisteredMsg("MODE", nickName);
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("ModeRequest Invalid - NotRegistered");

        return false;
    }

    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(channelName);

    // 채널이 없는 경우
    if (!channel)
    {
        std::string errorMessage = buildNoSuchChannelMsg(nickName, channelName);
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("ModeRequest Invalid - NoSuchChannel");

        return false;
    }

    // 채널 operator가 아닌 경우
    if (!channel->CheckClientIsOperator(nickName))
    {
        std::string errorMessage = buildNotChannelOperatorMsg(nickName, channelName);
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("ModeRequest Invalid - NotChannelOperator");

        return false;
    }

    if (modeChar == "o")
        return validateOperUserMode(client, channel, modeRequest);
    else if (modeChar == "l")
        return validateClientLimitMode(channel, modeRequest);
    else if (modeChar == "i")
        return validateInviteOnlyMode(channel, modeRequest);
    else if (modeChar == "k")
        return validateKeyMode(client, channel, modeRequest);
    else
        return validateProtectedTopicMode(channel, modeRequest);
}

// NICK Command 경우의 수 검증 완료
bool Validator::Visit(NickRequest *nickRequest) const
{
    Client *client = nickRequest->GetClient();
    const std::string &nickName = client->GetNickName();

    // 자신의 닉네임과 동일한 경우 동작 X
    if (nickName == nickRequest->GetNickName())
    {
        LOG_TRACE("NickRequest Invalid - SameAsSelf");

        return false;
    }

    ClientRepository *clientRepository = ClientRepository::GetInstance();

    // 이미 해당 닉네임을 사용하는 클라이언트가 존재하는 경우
    if (clientRepository->FindByNickName(nickName))
    {
        std::string errorMessage = buildNickNameInUseMsg(nickRequest->GetNickName(), nickName);
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("NickRequest Invalid - NickNameIsUse");

        return false;
    }

    // Connection Password를 입력하지 않고 Register 하려는 경우
    if (client->HasEnteredUserInfo() && !client->HasEnteredPassword())
    {
        std::string errorMessage;
        errorMessage = buildAccessDeniedMsg(client->GetUserName(), client->GetHostName());

        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("NickRequest Invalid - AccessDenied");

        return false;
    }

    LOG_TRACE("NickRequest Validated");

    return true;
}

bool Validator::Visit(PartRequest *partRequest) const
{
    Client *client = partRequest->GetClient();
    const std::string &nickName = client->GetNickName();
    const std::string &channelName = partRequest->GetChannelName();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage = buildNotRegisteredMsg("PART", nickName);
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("PartRequest Invalid - NotRegistered");

        return false;
    }

    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(channelName);

    // 채널이 없는 경우
    if (!channel)
    {
        std::string errorMessage;
        errorMessage = buildNoSuchChannelMsg(nickName, channelName);

        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("PartRequest Invalid - NoSuchChannel");

        return false;
    }

    // 채널에 속하지 않는 경우
    if (!channel->CheckClientIsExist(nickName))
    {
        std::string errorMessage;
        errorMessage = buildNotOnChannelMsg(nickName, channelName);

        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("PartRequest Invalid - NotOnChannel");

        return false;
    }

    LOG_TRACE("PartRequest Validated");

    return true;
}

// User Command 경우의 수 검증 완료
bool Validator::Visit(PassRequest *passRequest) const
{
    Client *client = passRequest->GetClient();

    // Registered 한 경우
    if (client->HasRegistered())
    {
        std::string errorMessage = buildAlreadyRegisteredMsg(client->GetNickName());
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("PassRequest Invalid - AlreadyRegistered");

        return false;
    }

    EnvManager *envManager = EnvManager::GetInstance();

    // 비밀번호가 일치하지 않는 경우
    if (passRequest->GetPassword() != envManager->GetConnectionPassord())
    {
        std::string errorMessage = buildAccessDeniedMsg(client->GetUserName(), client->GetHostName());
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("PassRequest Invalid - AccessDenied");

        return false;
    }

    LOG_TRACE("PassRequest Validated");

    return true;
}

bool Validator::Visit(PingRequest *pingRequest) const
{
    Client *client = pingRequest->GetClient();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage = buildNotRegisteredMsg("PING", client->GetNickName());
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("PingRequest Invalid - NotRegistered");

        return false;
    }

    LOG_TRACE("PingRequest Validated");

    return true;
}

bool Validator::Visit(PrivmsgRequest *privmsgRequest) const
{
    Client *client = privmsgRequest->GetClient();
    const std::string &nickName = client->GetNickName();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage = buildNotRegisteredMsg("PRIVMSG", nickName);
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("PrivmsgRequest Invalid - NotRegistered");

        return false;
    }

    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    ClientRepository *clientRepository = ClientRepository::GetInstance();

    std::vector<std::string> validatedTargets;
    std::vector<std::string> &targets = privmsgRequest->GetTargets();
    std::vector<std::string>::iterator iter;

    for (iter = targets.begin(); iter != targets.end(); iter++)
    {
        if (iter->front() == '#')
        {
            Channel *targetChannel = channelRepository->FindByName(*iter);

            // 채널이 없는 경우
            if (!targetChannel)
            {
                std::string errorMessage;
                errorMessage = buildNoSuchChannelMsg(nickName, *iter);

                client->AddResponseToBuf(errorMessage);

                LOG_TRACE("PrivmsgRequest Invalid - NoSuchChannel");
            }
            else if (!targetChannel->CheckClientIsExist(nickName))
            {
                // 채널에 속하지 않는 경우
                std::string errorMessage;
                errorMessage = buildCannotSendToChannelMsg(nickName, *iter);

                client->AddResponseToBuf(errorMessage);

                LOG_TRACE("PrivmsgRequest Invalid - CannotSendToChannel");
            }
            else
            {
                validatedTargets.push_back(*iter);
            }
        }
        else
        {
            Client *targetClient = clientRepository->FindByNickName(*iter);

            // 닉네임이 존재하지 않는 경우
            if (!targetClient)
            {
                std::string errorMessage;
                errorMessage = buildNoSuchNickMsg(nickName, *iter);

                client->AddResponseToBuf(errorMessage);

                LOG_TRACE("PrivmsgRequest Invalid - NoSuchNick");
            }
            else
            {
                validatedTargets.push_back(*iter);
            }
        }
    }

    if (validatedTargets.empty())
    {
        LOG_TRACE("PrivmsgRequest Invalid - ValidatedTargetsEmpty");
        return false;
    }
    else
    {
        targets = validatedTargets;

        LOG_TRACE("PrivmsgRequest Validated");
        return true;
    }
}

bool Validator::Visit(QuitRequest *quitRequest) const
{
    LOG_TRACE("QuitRequest Validated");

    return true;
}

bool Validator::Visit(TopicRequest *topicRequest) const
{
    Client *client = topicRequest->GetClient();
    const std::string &nickName = client->GetNickName();
    const std::string &channelName = topicRequest->GetChannelName();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage = buildNotRegisteredMsg("TOPIC", nickName);
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("TopicRequest Invalid - NotRegistered");

        return false;
    }

    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(channelName);

    // 채널이 없는 경우
    if (!channel)
    {
        std::string errorMessage;
        errorMessage = buildNoSuchChannelMsg(nickName, channelName);

        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("TopicRequest Invalid - NoSuchChannel");

        return false;
    }

    // 채널에 속하지 않는 경우
    if (!channel->CheckClientIsExist(nickName))
    {
        std::string errorMessage;
        errorMessage = buildNotOnChannelMsg(nickName, channelName);

        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("TopicRequest Invalid - NotOnChannel");

        return false;
    }

    // 채널이 ProtectedTopic 모드일때 ChannelOperator가 아닌 경우
    if (channel->IsProtectedTopicMode() && !channel->CheckClientIsOperator(nickName))
    {
        std::string errorMessage;
        errorMessage = buildNotChannelOperatorMsg(nickName, channelName);

        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("TopicRequest Invalid - NotChannelOperator");

        return false;
    }

    // 채널의 Topic과 변경을 요청한 Topic이 동일한 경우
    if (topicRequest->GetTopic() == channel->GetTopic())
        return false;

    LOG_TRACE("TopicRequest Validated");

    return true;
}

// User Command 경우의 수 검증 완료
bool Validator::Visit(UserRequest *userRequest) const
{
    Client *client = userRequest->GetClient();

    // 이미 UserInfo를 입력한 경우
    if (client->HasEnteredUserInfo())
    {
        std::string errorMessage = buildAlreadyRegisteredMsg(client->GetNickName());
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("UserRequest Invalid - AlreadyRegistered");

        return false;
    }

    // Connection Password를 입력하지 않고 Register 하려는 경우
    if (client->HasEnteredNickName() && !client->HasEnteredPassword())
    {
        std::string errorMessage = buildAccessDeniedMsg(userRequest->GetUserName(), userRequest->GetHostName());
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("UserRequest Invalid - AccessDenied");

        return false;
    }

    LOG_TRACE("UserRequest Validated");

    return true;
}

std::string Validator::buildAlreadyRegisteredMsg(const std::string &nickName) const
{
    EnvManager *envManager = EnvManager::GetInstance();
    std::stringstream errorMessage;

    errorMessage << ":" << envManager->GetServerName() << " 462 " << (nickName.empty() ? "*" : nickName)
                 << ":You may not reregister";

    return errorMessage.str();
}

// TODO 원래는 GetHostName()이 아니라 아이피 주소로 출력됨 방법 찾기
// TODO AccessDenied 는 연결을 끊어야 함!!!!!
std::string Validator::buildAccessDeniedMsg(const std::string &userName, const std::string &hostName) const
{
    std::stringstream errorMessage;
    errorMessage << "Error :Closing link: (" << (userName.empty() ? "*" : userName) << "@"
                 << (hostName.empty() ? "*" : hostName) << ") [Access denied by configuration]";

    return errorMessage.str();
}

std::string Validator::buildNickNameInUseMsg(const std::string &newNickName, const std::string &clientNickName) const
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream errorMessage;
    errorMessage << ":" << envManager->GetServerName() << " 433 " << (clientNickName.empty() ? "*" : clientNickName)
                 << " " << newNickName << " :Nickname is already in use.";

    return errorMessage.str();
}

std::string Validator::buildNotRegisteredMsg(const std::string &commandType, const std::string &nickName) const
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream errorMessage;
    errorMessage << ":" << envManager->GetServerName() << " 451 " << (nickName.empty() ? "*" : nickName) << " "
                 << commandType << " :You have not registered.";

    return errorMessage.str();
}

std::string Validator::buildNoSuchChannelMsg(const std::string &nickName, const std::string &channelName) const
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream errorMessage;
    errorMessage << ":" << envManager->GetServerName() << " 403 " << nickName << " " << channelName
                 << " :No such channel";

    return errorMessage.str();
}

std::string Validator::buildNoSuchNickMsg(const std::string &nickName, const std::string &targetNickName) const
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream errorMessage;
    errorMessage << ":" << envManager->GetServerName() << " 401 " << nickName << " " << targetNickName
                 << " :No such nick";

    return errorMessage.str();
}

std::string Validator::buildNotOnChannelMsg(const std::string &nickName, const std::string &channelName) const
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream errorMessage;
    errorMessage << ":" << envManager->GetServerName() << " 442 " << nickName << " " << channelName
                 << " :You're not on that channel!";

    return errorMessage.str();
}

std::string Validator::buildUserOnChannelMsg(const std::string &nickName, const std::string &targetNickName,
                                             const std::string &channelName) const
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream errorMessage;
    errorMessage << ":" << envManager->GetServerName() << " 443 " << nickName << " " << targetNickName << " "
                 << channelName << " :is already on channel";

    return errorMessage.str();
}

std::string Validator::buildUserNotInChannelMsg(const std::string &nickName, const std::string &targetNickName,
                                                const std::string &channelName) const
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream errorMessage;
    errorMessage << ":" << envManager->GetServerName() << " 441 " << nickName << " " << targetNickName << " "
                 << channelName << " :They are not on that channel";

    return errorMessage.str();
}

std::string Validator::buildNotChannelOperatorMsg(const std::string &nickName, const std::string &channelName) const
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream errorMessage;
    errorMessage << ":" << envManager->GetServerName() << " 482 " << nickName << " " << channelName
                 << " :You You must be a channel operator";

    return errorMessage.str();
}

std::string Validator::buildCannotSendToChannelMsg(const std::string &nickName, const std::string &channelName) const
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream errorMessage;
    errorMessage << ":" << envManager->GetServerName() << " 404 " << nickName << " " << channelName
                 << " :You cannot send external messages to this channel";

    return errorMessage.str();
}

std::string Validator::buildKeySetMsg(const std::string &nickName, const std::string &channelName) const
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream errorMessage;
    errorMessage << ":" << envManager->GetServerName() << " 467 " << nickName << " " << channelName
                 << " :Channel key already set";

    return errorMessage.str();
}

bool Validator::validateOperUserMode(Client *client, Channel *channel, ModeRequest *modeRequest) const
{
    const std::string &nickName = client->GetNickName();
    const std::string &channelName = modeRequest->GetChannelName();
    const std::string &targetNickName = modeRequest->GetModeArgument();

    ClientRepository *clientRepository = ClientRepository::GetInstance();
    Client *targetClient = clientRepository->FindByNickName(targetNickName);

    // 채널 operator 권한을 부여할 클라이언트가 존재하지 않는 경우
    if (!targetClient)
    {
        std::string errorMessage = buildNoSuchNickMsg(nickName, targetNickName);
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("ModeRequest Invalid - NoSuchNick");

        return false;
    }

    // 채널 operator 권한을 부여할 클라이언트가 채널에 없는 경우
    if (!channel->CheckClientIsExist(targetNickName))
    {
        std::string errorMessage = buildUserNotInChannelMsg(nickName, targetNickName, channelName);
        client->AddResponseToBuf(errorMessage);

        LOG_TRACE("ModeRequest Invalid - UserNotInChannel");

        return false;
    }

    const std::string &sign = modeRequest->GetSign();
    bool targetIsOperator = channel->CheckClientIsOperator(targetNickName);

    // sign + 일때 이미 대상 클라이언트가 채널 operator 이거나, sign - 일때 대상이 operator가 아닌 경우
    if ((targetIsOperator && (sign == "+")) || (!targetIsOperator && (sign == "-")))
    {
        LOG_TRACE("ModeRequest Invalid - NoAction");

        return false;
    }

    LOG_TRACE("ModeRequest Validated");

    return true;
}

bool Validator::validateClientLimitMode(Channel *channel, ModeRequest *modeRequest) const
{
    const std::string &sign = modeRequest->GetSign();

    if (sign == "+" && channel->IsClientLimitMode())
    {
        const std::string &limitString = modeRequest->GetModeArgument();
        int clientLimit = atoi(limitString.c_str());

        if (clientLimit == channel->GetClientLimit())
        {
            LOG_TRACE("ModeRequest Invalid - NoAction");

            return false;
        }
    }

    if (sign == "-" && !channel->IsClientLimitMode())
    {
        LOG_TRACE("ModeRequest Invalid - NoAction");

        return false;
    }

    LOG_TRACE("ModeRequest Validated");

    return true;
}

bool Validator::validateInviteOnlyMode(Channel *channel, ModeRequest *modeRequest) const
{
    const std::string &sign = modeRequest->GetSign();

    if ((sign == "+" && channel->IsInviteOnlyMode()) && (sign == "-" && !channel->IsInviteOnlyMode()))
    {
        LOG_TRACE("ModeRequest Invalid - NoAction");

        return false;
    }

    LOG_TRACE("ModeRequest Validated");

    return true;
}

bool Validator::validateKeyMode(Client *client, Channel *channel, ModeRequest *modeRequest) const
{
    const std::string &sign = modeRequest->GetSign();

    if ((sign == "+" && channel->IsKeyMode()) && (sign == "-" && !channel->IsKeyMode()))
    {
        LOG_TRACE("ModeRequest Invalid - NoAction");

        return false;
    }

    if (sign == "-" && channel->IsKeyMode())
    {
        const std::string &keyValue = modeRequest->GetModeArgument();

        if (keyValue != channel->GetKey())
        {
            std::string errorMessage = buildKeySetMsg(client->GetNickName(), modeRequest->GetChannelName());
            client->AddResponseToBuf(errorMessage);

            LOG_TRACE("ModeRequest Invalid - KeySet");

            return false;
        }
    }

    LOG_TRACE("ModeRequest Validated");

    return true;
}

bool Validator::validateProtectedTopicMode(Channel *channel, ModeRequest *modeRequest) const
{
    const std::string &sign = modeRequest->GetSign();

    if ((sign == "+" && channel->IsProtectedTopicMode()) && (sign == "-" && !channel->IsProtectedTopicMode()))
    {
        LOG_TRACE("ModeRequest Invalid - NoAction");

        return false;
    }

    LOG_TRACE("ModeRequest Validated");

    return true;
}
