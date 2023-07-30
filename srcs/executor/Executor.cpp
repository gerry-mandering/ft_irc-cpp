#include "Executor.hpp"

bool Executor::Visit(CapRequest *capRequest) const
{
    LOG_TRACE("CapRequest Executed");

    return true;
}

bool Executor::Visit(InviteRequest *inviteRequest) const
{
    LOG_TRACE("InviteRequest Executed");

    return true;
}

bool Executor::Visit(JoinRequest *joinRequest) const
{
    LOG_TRACE("JoinRequest Executed");

    return true;
}

bool Executor::Visit(KickRequest *kickRequest) const
{
    LOG_TRACE("KickRequest Executed");

    return true;
}

bool Executor::Visit(ModeRequest *modeRequest) const
{
    LOG_TRACE("ModeRequest Executed");

    return true;
}

bool Executor::Visit(NickRequest *nickRequest) const
{
    ClientRepository *clientRepository = ClientRepository::GetInstance();
    Client *client = nickRequest->GetClient();

    // TODO NickNameToClients 맵에 넣어주는 시점 적당한지 검증해야함
    if (!client->HasEnteredNickName())
    {
        client->SetNickName(nickRequest->GetNickName());
        clientRepository->AddClientToNickNameMap(client);
        client->SetNickNameEntered();
    }
    else
    {
        clientRepository->RemoveClientFromNickNameMap(client->GetNickName());
        client->SetNickName(nickRequest->GetNickName());
        clientRepository->AddClientToNickNameMap(client);
    }

    if (client->HasRegistered())
    {
        std::stringstream responseMessage;
        responseMessage << client->GetClientInfo() << "NICK :" << nickRequest->GetNickName();

        Channel *channel = client->GetChannel();
        if (channel)
            channel->BroadcastMessage(responseMessage.str());
        else
            client->InsertResponse(responseMessage.str());

        LOG_TRACE("NickRequest Executing - Registered User Changed NickName");
    }

    if (!client->HasRegistered() && client->HasEnteredUserInfo() && client->HasEnteredPassword())
    {
        // TODO Welcome message 뿌려주기
        EnvManager *envManager = EnvManager::GetInstance();

        std::stringstream welcomeMessage;
        welcomeMessage << ":" << envManager->GetServerName() << " 001 " << client->GetNickName()
                       << " :Welcome to the PingPong IRC Network " << client->GetClientInfo();

        client->InsertResponse(welcomeMessage.str());
        client->SetRegistered();

        LOG_TRACE("NickRequest Executing - SetRegistered");
    }

    LOG_TRACE("NickRequest Executed");

    return true;
}

bool Executor::Visit(PartRequest *partRequest) const
{
    Client *client = partRequest->GetClient();

    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(partRequest->GetChannelName());

    std::stringstream responseMessage;
    responseMessage << client->GetClientInfo() << " PART " << partRequest->GetChannelName();

    if (!partRequest->GetReason().empty())
        responseMessage << " :" << partRequest->GetReason();

    channel->BroadcastMessage(responseMessage.str());

    // Operator 이면 지워주는 것은 내부에서 알아서 해줌
    channel->RemoveClient(client->GetNickName());
    // TODO Shared Ptr 이면 delete?
    client->SetChannel(NULL);

    LOG_TRACE("PartRequest Executed");

    return true;
}

bool Executor::Visit(PassRequest *passRequest) const
{
    Client *client = passRequest->GetClient();

    client->SetPasswordEntered();

    LOG_TRACE("PassRequest Executed");

    return true;
}

bool Executor::Visit(PingRequest *pingRequest) const
{
    EnvManager *envManager = EnvManager::GetInstance();
    std::string serverName = envManager->GetServerName();

    std::stringstream responseMessage;
    responseMessage << ":" << serverName << " PONG " << serverName << " :" << pingRequest->GetToken();

    Client *client = pingRequest->GetClient();
    client->InsertResponse(responseMessage.str());

    LOG_TRACE("PingRequest Executed");

    return true;
}

bool Executor::Visit(PrivmsgRequest *privmsgRequest) const
{
    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    ClientRepository *clientRepository = ClientRepository::GetInstance();

    Client *client = privmsgRequest->GetClient();
    std::stringstream responseMessage;

    std::vector<std::string> targets = privmsgRequest->GetTargets();
    std::vector<std::string>::iterator iter;

    for (iter = targets.begin(); iter != targets.end(); iter++)
    {
        responseMessage << client->GetClientInfo() << " PRIVMSG " << *iter << ":" << privmsgRequest->GetMessage();

        if (iter->front() == '#')
        {
            Channel *targetChannel = channelRepository->FindByName(*iter);
            targetChannel->BroadcastMessage(responseMessage.str());
        }
        else
        {
            Client *targetClient = clientRepository->FindByNickName(*iter);
            targetClient->InsertResponse(responseMessage.str());
        }
    }

    LOG_TRACE("PrivmsgRequest Executed");

    return true;
}

bool Executor::Visit(QuitRequest *quitRequest) const
{
    Client *client = quitRequest->GetClient();

    std::stringstream responseMessage;

    // TODO hostname 수정
    responseMessage << "ERROR :Closing link: (" << client->GetUserName() << "@" << client->GetHostName() << ")";

    if (quitRequest->GetReason().empty())
        responseMessage << " [Client exited]";
    else
        responseMessage << " [Quit: " << quitRequest->GetReason() << "]";

    client->InsertResponse(responseMessage.str());

    Channel *channel = client->GetChannel();

    if (channel)
    {
        std::stringstream responseMessage;

        responseMessage << client->GetClientInfo() << " QUIT :";

        if (quitRequest->GetReason().empty())
            responseMessage << "Client exited";
        else
            responseMessage << "Quit : " << quitRequest->GetReason();

        channel->BroadcastMessage(responseMessage.str());
        channel->RemoveClient(client->GetNickName());

        LOG_TRACE("QuitRequest Executing - BroadcastMessage");
    }

    ClientRepository *clientRepository = ClientRepository::GetInstance();

    clientRepository->RemoveClient(client->GetSocket(), client->GetNickName());

    // TODO 소켓 닫아주기 - Client 소멸자에서 하는 방식으로 구성?

    LOG_TRACE("QuitRequest Executed");

    return true;
}

bool Executor::Visit(TopicRequest *topicRequest) const
{
    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(topicRequest->GetChannelName());

    std::string newTopic = topicRequest->GetTopic();

    if (newTopic == channel->GetTopic())
        return true;

    channel->SetTopic(newTopic);

    std::stringstream topicChangedMsg;
    topicChangedMsg << topicRequest->GetClient()->GetClientInfo() << "TOPIC " << topicRequest->GetChannelName() << " :"
                    << newTopic;

    channel->BroadcastMessage(topicChangedMsg.str());

    LOG_TRACE("TopicRequest Executed");

    return true;
}

bool Executor::Visit(UserRequest *userRequest) const
{
    Client *client = userRequest->GetClient();

    client->SetUserName(userRequest->GetUserName());
    client->SetHostName(userRequest->GetHostName());
    client->SetServerName(userRequest->GetServerName());
    client->SetRealName(userRequest->GetRealName());
    client->SetUserInfoEntered();

    if (client->HasEnteredNickName() && client->HasEnteredPassword())
    {
        // TODO Welcome message 뿌려주기
        EnvManager *envManager = EnvManager::GetInstance();

        std::stringstream welcomeMessage;
        welcomeMessage << ":" << envManager->GetServerName() << " 001 " << client->GetNickName()
                       << " :Welcome to the PingPong IRC Network " << client->GetClientInfo();

        client->InsertResponse(welcomeMessage.str());
        client->SetRegistered();

        LOG_TRACE("UserRequest Executing - SetRegistered");
    }

    LOG_TRACE("UserRequest Executed");

    return true;
}