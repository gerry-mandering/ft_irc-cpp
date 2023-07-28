#include "Executor.hpp"

bool Executor::Visit(CapRequest *capRequest) const
{

    return true;
}

bool Executor::Visit(InviteRequest *inviteRequest) const
{

    return true;
}

bool Executor::Visit(JoinRequest *joinRequest) const
{

    return true;
}

bool Executor::Visit(KickRequest *kickRequest) const
{

    return true;
}

bool Executor::Visit(ModeRequest *modeRequest) const
{

    return true;
}

// TODO 클라이언트가 속한 채널 가져오기
bool Executor::Visit(NickRequest *nickRequest) const
{
    Client *client = nickRequest->GetClient();

    client->SetNickName(nickRequest->GetNickName());
    client->SetNickNameEntered();

    if (client->HasRegistered())
    {
        std::stringstream responseMessage;
        responseMessage << client->GetClientInfo() << "NICK :" << nickRequest->GetNickName();

        Channel *channel = client->GetChannel();
        if (channel)
            channel->BroadcastMessage(responseMessage.str());
        else
            client->InsertResponse(responseMessage.str());
    }

    if (!client->HasRegistered() && client->HasEnteredUserInfo() && client->HasEnteredPassword())
    {
        // TODO Welcome message 뿌려주기
        client->SetRegistered();
    }

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

    channel->RemoveClient(client);
    client->SetChannel(NULL);

    return true;
}

bool Executor::Visit(PassRequest *passRequest) const
{
    Client *client = passRequest->GetClient();

    client->SetPasswordEntered();

    return true;
}

bool Executor::Visit(PingRequest *pingRequest) const
{
    // TODO EnvManager로 부터 서버이름 받아오기
    std::stringstream responseMessage;
    responseMessage << ":irc.local PONG irc.local :" << pingRequest->GetToken();

    Client *client = pingRequest->GetClient();
    client->InsertResponse(responseMessage.str());

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

            Client *targetClient = clientRepository->FindByNickname(*iter);
            targetClient->InsertResponse(responseMessage.str());
        }
    }

    return true;
}

bool Executor::Visit(QuitRequest *quitRequest) const
{

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

    if (!client->HasRegistered() && client->HasEnteredNickName() && client->HasEnteredPassword())
    {
        // TODO Welcome message 뿌려주기
        client->SetRegistered();
    }

    return true;
}
