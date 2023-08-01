#include "Executor.hpp"
#include "ChannelRepository.hpp"
#include "Reactor.hpp"
#include "def.h"

bool Executor::Visit(CapRequest *capRequest) const
{
    LOG_TRACE("CapRequest Executed");

    return true;
}

bool Executor::Visit(InviteRequest *inviteRequest) const
{
    ClientRepository *clientRepository = ClientRepository::GetInstance();
    Client *client = inviteRequest->GetClient();

    const std::string &nickName = client->GetNickName();
    const std::string &targetNickName = inviteRequest->GetNickName();
    const std::string &channelName = inviteRequest->GetChannelName();

    // RPL_INVITING 메세지 요청자에게 보내기
    std::string replyInvitingMessage = buildReplyInvitingMsg(nickName, targetNickName, channelName);
    client->AddResponseToBuf(replyInvitingMessage);

    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(channelName);

    // 채널에 속한 클라이언트들에게 초대 알리기
    std::string InvitedIntoChannelMessage = buildInvitedIntoChannelMsg(nickName, targetNickName, channelName);
    channel->BroadcastMessageExcludingRequestor(InvitedIntoChannelMessage, nickName);

    Client *targetClient = clientRepository->FindByNickName(targetNickName);

    // 초대를 받은 사람에게 초대장 메세지 보내기
    std::string invitationMessage = buildInvitationMsg(client, targetNickName, channelName);
    targetClient->AddResponseToBuf(invitationMessage);

    channel->AddToInvitedClient(targetClient);

    LOG_TRACE("InviteRequest Executed");

    return true;
}

// TODO InvitedClients vector에 있으면 제거
bool Executor::Visit(JoinRequest *joinRequest) const
{
    ChannelRepository *channelRepo = ChannelRepository::GetInstance();
    Channel *channel;
    Client *client = joinRequest->GetClient();

    LOG_TRACE("JoinRequest Executing");

    channel = channelRepo->FindByName(joinRequest->getChannelName());
    // 채널에 처음 입장할 때
    if (!channel)
    {
        LOG_TRACE("First Join: create new channel");
        channel = channelRepo->CreateChannel(joinRequest->getChannelName());
        LOG_TRACE("call SetClient, SetOperator");
        channel->SetClient(client);
        channel->SetOperator(client);
        return true;
    }
    LOG_TRACE("Join already existing channel");
    channel->SetClient(client);
    return true;

    // 레거시.. 지울게요
    // only invite mode일 때
    // if (channel->IsInviteOnlyMode())
    // {
    //     LOG_TRACE("channel is invite only mode");
    //     return true;
    // }

    // // key mode일 때
    // if (channel->IsKeyMode())
    // {
    //     LOG_TRACE("channel is key mode");
    //     if (channel->GetKey() != joinRequest->GetKey())
    //     {
    //         LOG_TRACE("key is not matched");
    //         return true;
    //     }
    //     LOG_TRACE("key is matched");
    //     join_only_if_not_in(client, channel);
    //     return (true);
    // }

    // if (channel->IsClientLimitMode() && channel->GetClientLimit() <= channel->GetClientCount())
    // {
    //     LOG_TRACE("channel is full");
    //     return true;
    // }
    // // 1. 아무런 모드도 설정되지 않았거나, limit mode이면서 limit에 도달하지 않았을 때
    // else
    // {
    //     LOG_TRACE("client limit mode but not full, try join");
    //     join_only_if_not_in(client, channel);
    //     return true;
    // }
}

bool Executor::Visit(KickRequest *kickRequest) const
{
    const std::string &channelName = kickRequest->GetChannelName();
    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(channelName);

    Client *client = kickRequest->GetClient();
    std::string responseMessage;

    std::vector<std::string> targets = kickRequest->GetTargets();
    std::vector<std::string>::iterator iter;

    for (iter = targets.begin(); iter != targets.end(); iter++)
    {
        responseMessage = buildKickoutMsg(client, channelName, *iter, kickRequest->GetMessage());

        channel->BroadcastMessage(responseMessage);
        channel->RemoveClient(*iter);
    }

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
        std::string nickChangedMessage = buildNickChangedMsg(client, nickRequest->GetNickName());

        Channel *channel = client->GetChannel();
        if (channel)
            channel->BroadcastMessage(nickChangedMessage);
        else
            client->AddResponseToBuf(nickChangedMessage);

        LOG_TRACE("NickRequest Executing - Registered User Changed NickName");
    }

    if (!client->HasRegistered() && client->HasEnteredUserInfo() && client->HasEnteredPassword())
    {
        client->AddResponseToBuf(buildWelcomeMsg(client));
        client->SetRegistered();

        LOG_TRACE("NickRequest Executing - SetRegistered");
    }

    LOG_TRACE("NickRequest Executed");

    return true;
}

bool Executor::Visit(PartRequest *partRequest) const
{
    const std::string &channelName = partRequest->GetChannelName();
    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(channelName);

    Client *client = partRequest->GetClient();
    std::string partMessage = buildPartMsg(client, channelName, partRequest->GetReason());

    channel->BroadcastMessage(partMessage);
    channel->RemoveClient(client->GetNickName());
    client->SetChannel(NULL);
    // TODO Shared Ptr 이면 delete?

    // TODO channel에 사용자가 다 나가면 repository에서 비워주고 channel delete

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
    std::string pongMessage = buildPongMsg(pingRequest->GetToken());

    pingRequest->GetClient()->AddResponseToBuf(pongMessage);

    LOG_TRACE("PingRequest Executed");

    return true;
}

bool Executor::Visit(PrivmsgRequest *privmsgRequest) const
{
    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    ClientRepository *clientRepository = ClientRepository::GetInstance();

    Client *client = privmsgRequest->GetClient();
    std::string privateMessage;

    std::vector<std::string> targets = privmsgRequest->GetTargets();
    std::vector<std::string>::iterator iter;

    for (iter = targets.begin(); iter != targets.end(); iter++)
    {
        privateMessage = buildPrivateMsg(client, *iter, privmsgRequest->GetMessage());

        if (iter->front() == '#')
        {
            Channel *targetChannel = channelRepository->FindByName(*iter);
            targetChannel->BroadcastMessage(privateMessage);
        }
        else
        {
            Client *targetClient = clientRepository->FindByNickName(*iter);
            targetClient->AddResponseToBuf(privateMessage);
        }
    }

    LOG_TRACE("PrivmsgRequest Executed");

    return true;
}

bool Executor::Visit(QuitRequest *quitRequest) const
{
    Client *client = quitRequest->GetClient();

    std::string closingLinkMessage = buildClosingLinkMsg(client, quitRequest->GetReason());

    client->AddResponseToBuf(closingLinkMessage);

    Channel *channel = client->GetChannel();
    if (channel)
    {
        std::string quitMessage = buildQuitMsg(client, quitRequest->GetReason());

        channel->BroadcastMessage(quitMessage);
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

    std::string topic = topicRequest->GetTopic();
    channel->SetTopic(topic);

    std::string topicChangedMessage =
        buildTopicChangedMsg(topicRequest->GetClient(), topicRequest->GetChannelName(), topic);

    channel->BroadcastMessage(topicChangedMessage);

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
        client->AddResponseToBuf(buildWelcomeMsg(client));
        client->SetRegistered();

        LOG_TRACE("UserRequest Executing - SetRegistered");
    }

    LOG_TRACE("UserRequest Executed");

    return true;
}

std::string Executor::buildNickChangedMsg(Client *client, const std::string &nickName)
{
    std::stringstream nickChangedMessage;

    nickChangedMessage << ":" << client->GetClientInfo() << " NICK "
                       << ":" << nickName;

    return nickChangedMessage.str();
}

std::string Executor::buildWelcomeMsg(Client *client)
{
    EnvManager *envManager = EnvManager::GetInstance();
    ClientRepository *clientRepository = ClientRepository::GetInstance();
    ChannelRepository *channelRepository = ChannelRepository::GetInstance();

    std::string serverName = envManager->GetServerName();

    std::stringstream welcomeMessage;
    welcomeMessage
        << ":" << serverName << " 001 " << client->GetNickName() << " :Welcome to the PingPong IRC Network "
        << client->GetClientInfo() << "\r\n"
        << ":" << serverName << " 002 " << client->GetNickName() << " :Your host is " << serverName
        << ", running version InspIRCd-3\r\n"
        << ":" << serverName << " 003 " << client->GetNickName() << " :This server was created "
        << envManager->GetServerBootTime() << "\r\n"
        << ":" << serverName << " 004 " << client->GetNickName() << " " << serverName << " InspIRCd-3 itkol\r\n"
        << ":" << serverName << " 255 " << client->GetNickName() << " :I have "
        << clientRepository->GetNumberOfClients() << " clients and " << channelRepository->GetNumberOfChannels()
        << " channels\r\n"
        << ":" << serverName << " 375 " << client->GetNickName() << " :" << serverName << " message of the day\r\n"
        << ":" << serverName << " 372 " << client->GetNickName() << " "
        << "__________.__                       __________                      ________                \r\n"
        << ":" << serverName << " 372 " << client->GetNickName() << " "
        << "\\______   |__| ____   ____          \\______   \\____   ____   ____   \\______ \\  __ __  ____  \r\n"
        << ":" << serverName << " 372 " << client->GetNickName() << " "
        << " |     ___|  |/    \\ / ___\\   ______ |     ___/  _ \\ /    \\ / ___\\   |    |  \\|  |  \\/  _ \\ \r\n"
        << ":" << serverName << " 372 " << client->GetNickName() << " "
        << " |    |   |  |   |  / /_/  > /_____/ |    |  (  <_> |   |  / /_/  >  |    `   |  |  (  <_> )\r\n"
        << ":" << serverName << " 372 " << client->GetNickName() << " "
        << " |____|   |__|___|  \\___  /          |____|   \\____/|___|  \\___  /  /_______  |____/ \\____/ \r\n"
        << ":" << serverName << " 372 " << client->GetNickName() << " "
        << "                  \\/_____/                               \\/_____/           \\/              \r\n"
        << ":" << serverName << " 376 " << client->GetNickName() << " :End of message of the day.\r\n";

    return welcomeMessage.str();
}

std::string Executor::buildReplyInvitingMsg(const std::string &nickName, const std::string &targetNickName,
                                            const std::string &channelName)
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream replyMessage;
    replyMessage << ":" << envManager->GetServerName() << " 341 " << nickName << " " << targetNickName << " "
                 << channelName;

    return replyMessage.str();
}

std::string Executor::buildInvitedIntoChannelMsg(const std::string &nickName, const std::string &targetNickName,
                                                 const std::string &channelName)
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream replyMessage;
    replyMessage << ":" << envManager->GetServerName() << " NOTICE " << channelName << " :*** " << nickName
                 << " invited " << targetNickName << " into the channel";

    return replyMessage.str();
}

std::string Executor::buildInvitationMsg(Client *client, const std::string &targetNickName,
                                         const std::string &channelName)
{
    std::stringstream invitationMessage;
    invitationMessage << client->GetClientInfo() << " INVITE " << targetNickName << " :" << channelName;

    return invitationMessage.str();
}

std::string Executor::buildKickoutMsg(Client *client, const std::string &channelName, const std::string &targetNickName,
                                      const std::string &message)
{
    std::stringstream kickoutMessage;
    kickoutMessage << client->GetClientInfo() << " KICK " << channelName << " " << targetNickName << " :" << message;

    return kickoutMessage.str();
}

std::string Executor::buildTopicChangedMsg(Client *client, const std::string &channelName, const std::string &topic)
{
    std::stringstream topicChangedMessage;
    topicChangedMessage << ":" << client->GetClientInfo() << " TOPIC " << channelName << " :" << topic;

    return topicChangedMessage.str();
}

std::string Executor::buildClosingLinkMsg(Client *client, const std::string &reason)
{
    std::stringstream closingLinkMessage;
    closingLinkMessage << "ERROR: Closing link: " << client->GetUserName() << "@" << client->GetHostName() << ") ";

    if (reason.empty())
        closingLinkMessage << "[Client exited]";
    else
        closingLinkMessage << "[Quit: " << reason << "]";

    return closingLinkMessage.str();
}

std::string Executor::buildQuitMsg(Client *client, const std::string &reason)
{
    std::stringstream quitMessage;
    quitMessage << ":" << client->GetClientInfo() << " Quit :";

    if (reason.empty())
        quitMessage << "Client exited";
    else
        quitMessage << "Quit: " << reason;

    return quitMessage.str();
}

std::string Executor::buildPrivateMsg(Client *client, const std::string &target, const std::string &message)
{
    std::stringstream privateMessage;
    privateMessage << ":" << client->GetClientInfo() << " PRIVMSG " << target << " :" << message;

    return privateMessage.str();
}

std::string Executor::buildPongMsg(const std::string &token)
{
    EnvManager *envManager = EnvManager::GetInstance();
    const std::string &serverName = envManager->GetServerName();

    std::stringstream pongMessage;
    pongMessage << ":" << serverName << " PONG " << serverName << " :" << token;

    return pongMessage.str();
}

std::string Executor::buildPartMsg(Client *client, const std::string &channelName, const std::string &reason)
{
    std::stringstream partMessage;
    partMessage << ":" << client->GetClientInfo() << " PART " << channelName;

    if (!reason.empty())
        partMessage << ":" << reason;

    return partMessage.str();
}