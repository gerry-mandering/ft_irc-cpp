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
 *
 * */

#include "Validator.hpp"

bool Validator::Visit(CapRequest *capRequest) const {}

bool Validator::Visit(InviteRequest *inviteRequest) const
{
    Client *client = inviteRequest->GetClient();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage;

        if (client->HasEnteredNickName())
            errorMessage = BuildNotRegisteredMsg("INVITE", client->GetNickName());
        else
            errorMessage = BuildNotRegisteredMsg("INVITE");

        client->InsertResponse(errorMessage);
        return false;
    }
}

bool Validator::Visit(JoinRequest *joinRequest) const
{
    Client *client = joinRequest->GetClient();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage;

        if (client->HasEnteredNickName())
            errorMessage = BuildNotRegisteredMsg("JOIN", client->GetNickName());
        else
            errorMessage = BuildNotRegisteredMsg("JOIN");

        client->InsertResponse(errorMessage);
        return false;
    }
}

bool Validator::Visit(KickRequest *kickRequest) const
{
    Client *client = kickRequest->GetClient();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage;

        if (client->HasEnteredNickName())
            errorMessage = BuildNotRegisteredMsg("KICK", client->GetNickName());
        else
            errorMessage = BuildNotRegisteredMsg("KICK");

        client->InsertResponse(errorMessage);
        return false;
    }
}

bool Validator::Visit(ModeRequest *modeRequest) const
{
    Client *client = modeRequest->GetClient();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage;

        if (client->HasEnteredNickName())
            errorMessage = BuildNotRegisteredMsg("MODE", client->GetNickName());
        else
            errorMessage = BuildNotRegisteredMsg("MODE");

        client->InsertResponse(errorMessage);
        return false;
    }
}

// NICK Command 경우의 수 검증 완료
bool Validator::Visit(NickRequest *nickRequest) const
{
    ClientRepository *clientRepository = ClientRepository::GetInstance();
    Client *client = nickRequest->GetClient();

    // 이미 해당 닉네임을 사용하는 클라이언트가 존재하는 경우
    if (clientRepository->FindByNickname(nickRequest->GetNickName()) != NULL)
    {
        std::string errorMessage;

        if (client->HasEnteredNickName())
            errorMessage = BuildNickNameInUseMsg(nickRequest->GetNickName(), client->GetNickName());
        else
            errorMessage = BuildNickNameInUseMsg(nickRequest->GetNickName());

        client->InsertResponse(errorMessage);
        return false;
    }

    // Connection Password를 입력하지 않고 Register 하려는 경우
    if (client->HasEnteredUserInfo() && !client->HasEnteredPassword())
    {
        std::string errorMessage;

        errorMessage = BuildAccessDeniedMsg(client->GetUserName(), client->GetHostName());

        client->InsertResponse(errorMessage);
        return false;
    }

    return true;
}

bool Validator::Visit(PartRequest *partRequest) const
{
    Client *client = partRequest->GetClient();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage;

        if (client->HasEnteredNickName())
            errorMessage = BuildNotRegisteredMsg("PART", client->GetNickName());
        else
            errorMessage = BuildNotRegisteredMsg("PART");

        client->InsertResponse(errorMessage);
        return false;
    }

    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(partRequest->GetChannelName());

    // 채널이 없는 경우
    if (!channel)
    {
        std::string errorMessage;

        errorMessage = BuildNoSuchChannelMsg(client->GetNickName(), partRequest->GetChannelName());

        client->InsertResponse(errorMessage);
        return false;
    }

    // 채널에 속하지 않는 경우
    if (!channel->CheckClientIsExist(client->GetNickName()))
    {
        std::string errorMessage;

        errorMessage = BuildNotOnChannelMsg(client->GetNickName(), partRequest->GetChannelName());

        client->InsertResponse(errorMessage);
        return false;
    }

    return true;
}

// User Command 경우의 수 검증 완료
bool Validator::Visit(PassRequest *passRequest) const
{
    Client *client = passRequest->GetClient();

    // Registered 하지 않은 경우
    if (client->HasRegistered())
    {
        std::string errorMessage;

        if (client->HasEnteredNickName())
            errorMessage = BuildAlreadyRegisteredMsg(client->GetNickName());
        else
            errorMessage = BuildAlreadyRegisteredMsg();

        client->InsertResponse(errorMessage);
        return false;
    }

    EnvManager *envManager = EnvManager::GetInstance();

    // 비밀번호가 일치하지 않는 경우
    if (passRequest->GetPassword() != envManager->GetConnectionPassord())
    {
        std::string errorMessage;

        if (client->HasEnteredUserInfo())
            errorMessage = BuildAccessDeniedMsg(client->GetUserName(), client->GetHostName());
        else
            errorMessage = BuildAccessDeniedMsg(); // TODO 사용자 hostname 은 가져와서 넣기

        client->InsertResponse(errorMessage);
        return false;
    }

    return true;
}

bool Validator::Visit(PingRequest *pingRequest) const
{
    Client *client = pingRequest->GetClient();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage;

        if (client->HasEnteredNickName())
            errorMessage = BuildNotRegisteredMsg("PING", client->GetNickName());
        else
            errorMessage = BuildNotRegisteredMsg("PING");

        client->InsertResponse(errorMessage);
        return false;
    }

    return true;
}

bool Validator::Visit(PrivmsgRequest *privmsgRequest) const
{
    Client *client = privmsgRequest->GetClient();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage;

        if (client->HasEnteredNickName())
            errorMessage = BuildNotRegisteredMsg("PRIVMSG", client->GetNickName());
        else
            errorMessage = BuildNotRegisteredMsg("PRIVMSG");

        client->InsertResponse(errorMessage);
        return false;
    }

    bool executeFlag = true;

    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    ClientRepository *clientRepository = ClientRepository::GetInstance();

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

                errorMessage = BuildNoSuchChannelMsg(client->GetNickName(), *iter);

                client->InsertResponse(errorMessage);
                targets.erase(iter);

                executeFlag ^= true;
            }

            // 채널에 속하지 않는 경우
            if (!targetChannel->CheckClientIsExist(client->GetNickName()))
            {
                std::string errorMessage;

                errorMessage = BuildCannotSendToChannelMsg(client->GetNickName(), *iter);

                client->InsertResponse(errorMessage);
                targets.erase(iter);

                executeFlag ^= true;
            }
        }
        else
        {
            Client *targetClient = clientRepository->FindByNickname(*iter);

            // 닉네임이 존재하지 않는 경우
            if (!targetClient)
            {
                std::string errorMessage;

                errorMessage = BuildNoSuchNickMsg(client->GetNickName(), *iter);

                client->InsertResponse(errorMessage);
                targets.erase(iter);

                executeFlag ^= true;
            }
        }
    }

    return executeFlag;
}

bool Validator::Visit(QuitRequest *quitRequest) const
{
    return true;
}

bool Validator::Visit(TopicRequest *topicRequest) const
{
    Client *client = topicRequest->GetClient();

    // Registered 하지 않은 경우
    if (!client->HasRegistered())
    {
        std::string errorMessage;

        if (client->HasEnteredNickName())
            errorMessage = BuildNotRegisteredMsg("TOPIC", client->GetNickName());
        else
            errorMessage = BuildNotRegisteredMsg("TOPIC");

        client->InsertResponse(errorMessage);
        return false;
    }

    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(topicRequest->GetChannelName());

    // 채널이 없는 경우
    if (!channel)
    {
        std::string errorMessage;

        errorMessage = BuildNoSuchChannelMsg(client->GetNickName(), topicRequest->GetChannelName());

        client->InsertResponse(errorMessage);
        return false;
    }

    // 채널에 속하지 않는 경우
    if (!channel->CheckClientIsExist(client->GetNickName()))
    {
        std::string errorMessage;

        errorMessage = BuildNotOnChannelMsg(client->GetNickName(), topicRequest->GetChannelName());

        client->InsertResponse(errorMessage);
        return false;
    }

    // 채널이 ProtectedTopic 모드일때 ChannelOperator가 아닌 경우
    if (channel->IsProtectedTopicMode() && !channel->CheckClientIsOperator(client->GetNickName()))
    {
        std::string errorMessage;

        errorMessage = BuildNotChannelOperatorMsg(client->GetNickName(), topicRequest->GetChannelName());

        client->InsertResponse(errorMessage);
        return false;
    }

    return true;
}

// User Command 경우의 수 검증 완료
bool Validator::Visit(UserRequest *userRequest) const
{
    Client *client = userRequest->GetClient();

    // 이미 UserInfo를 입력한 경우
    if (client->HasEnteredUserInfo())
    {
        std::string errorMessage;

        if (client->HasEnteredNickName())
            errorMessage = BuildAlreadyRegisteredMsg(client->GetNickName());
        else
            errorMessage = BuildAlreadyRegisteredMsg();

        client->InsertResponse(errorMessage);
        return false;
    }

    // Connection Password를 입력하지 않고 Register 하려는 경우
    if (client->HasEnteredNickName() && !client->HasEnteredPassword())
    {
        std::string errorMessage;

        // TODO hostname Ip로 알아오기
        errorMessage = BuildAccessDeniedMsg(userRequest->GetUserName(), userRequest->GetHostName());

        client->InsertResponse(errorMessage);
        return false;
    }

    return true;
}

std::string Validator::BuildAlreadyRegisteredMsg(const std::string &nickName)
{
    EnvManager *envManager = EnvManager::GetInstance();
    std::stringstream errorMessage;

    errorMessage << ":" << envManager->GetServerName() << " 462 " << nickName << ":You may not reregister";

    return errorMessage.str();
}

// TODO 원래는 GetHostName()이 아니라 아이피 주소로 출력됨 방법 찾기
// TODO AccessDenied 는 연결을 끊어야 함!!!!!
std::string Validator::BuildAccessDeniedMsg(const std::string &userName, const std::string &hostName)
{
    std::stringstream errorMessage;

    errorMessage << "Error :Closing link: (" << userName << "@" << hostName << ") [Access denied by configuration]";

    return errorMessage.str();
}

std::string Validator::BuildNickNameInUseMsg(const std::string &newNickName, const std::string &clientNickName)
{
    EnvManager *envManager = EnvManager::GetInstance();
    std::stringstream errorMessage;

    errorMessage << ":" << envManager->GetServerName() << " 433 " << clientNickName << " " << newNickName
                 << " :Nickname is already in use.";

    return errorMessage.str();
}

std::string Validator::BuildNotRegisteredMsg(const std::string &commandType, const std::string &nickName)
{
    EnvManager *envManager = EnvManager::GetInstance();
    std::stringstream errorMessage;

    errorMessage << ":" << envManager->GetServerName() << " 451 " << nickName << " " << commandType
                 << " :You have not registered.";

    return errorMessage.str();
}

std::string Validator::BuildNoSuchChannelMsg(const std::string &nickName, const std::string &channelName)
{
    EnvManager *envManager = EnvManager::GetInstance();
    std::stringstream errorMessage;

    errorMessage << ":" << envManager->GetServerName() << " 403 " << nickName << " " << channelName
                 << " :No such channel";

    return errorMessage.str();
}

std::string Validator::BuildNoSuchNickMsg(const std::string &nickName, const std::string &targetNickName)
{
    EnvManager *envManager = EnvManager::GetInstance();
    std::stringstream errorMessage;

    errorMessage << ":" << envManager->GetServerName() << " 401 " << nickName << " " << targetNickName
                 << " :No such nick";

    return errorMessage.str();
}

std::string Validator::BuildNotOnChannelMsg(const std::string &nickName, const std::string &channelName)
{
    EnvManager *envManager = EnvManager::GetInstance();
    std::stringstream errorMessage;

    errorMessage << ":" << envManager->GetServerName() << " 442 " << nickName << " " << channelName
                 << " :You're not on that channel!";

    return errorMessage.str();
}

std::string Validator::BuildNotChannelOperatorMsg(const std::string &nickName, const std::string &channelName)
{
    EnvManager *envManager = EnvManager::GetInstance();
    std::stringstream errorMessage;

    errorMessage << ":" << envManager->GetServerName() << " 482 " << nickName << " " << channelName
                 << " :You cannot send external messages to this channel.";

    return errorMessage.str();
}