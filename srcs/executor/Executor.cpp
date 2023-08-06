#include "Executor.hpp"
#include "ChannelRepository.hpp"
#include "Reactor.hpp"
#include "disconnect.h"

bool Executor::Visit(InviteRequest *inviteRequest) const
{
    ClientRepository *clientRepository = ClientRepository::GetInstance();
    SharedPtr< Client > client = inviteRequest->GetClient();

    const std::string &nickName = client->GetNickName();
    const std::string &targetNickName = inviteRequest->GetNickName();
    const std::string &channelName = inviteRequest->GetChannelName();

    // RPL_INVITING 메세지 요청자에게 보내기
    std::string replyInvitingMessage = buildReplyInvitingMsg(nickName, targetNickName, channelName);
    client->AddResponseToBuf(replyInvitingMessage);

    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    SharedPtr< Channel > channel = channelRepository->FindByName(channelName);

    // 채널에 속한 클라이언트들에게 초대 알리기
    std::string InvitedIntoChannelMessage = buildInvitedIntoChannelMsg(nickName, targetNickName, channelName);
    channel->BroadcastMessageExcludingRequestor(InvitedIntoChannelMessage, nickName);

    SharedPtr< Client > targetClient = clientRepository->FindByNickName(targetNickName);

    // 초대를 받은 사람에게 초대장 메세지 보내기
    std::string invitationMessage = buildInvitationMsg(client, targetNickName, channelName);
    targetClient->AddResponseToBuf(invitationMessage);

    channel->SetInvitedClient(targetClient);

    LOG_TRACE("InviteRequest Executed");

    return true;
}

bool Executor::Visit(JoinRequest *joinRequest) const
{
    ChannelRepository *channelRepo = ChannelRepository::GetInstance();
    SharedPtr< Channel > channel = channelRepo->FindByName(joinRequest->getChannelName());
    SharedPtr< Client > client = joinRequest->GetClient();

    LOG_TRACE("JoinRequest Executing");

    // 채널에 처음 입장할 때
    if (!channel)
    {
        LOG_TRACE("First Join: create new channel");

        channel = channelRepo->CreateChannel(joinRequest->getChannelName());

        LOG_TRACE("call SetClient, SetOperator");

        channel->SetClient(client);
        channel->SetOperator(client);
        client->SetChannel(channel);

        std::string responseMessage = buildJoinMsg(client, channel);
        client->AddResponseToBuf(responseMessage);

        return true;
    }
    LOG_TRACE("Join already existing channel");
    channel->SetClient(client);
    client->SetChannel(channel);
    channel->RemoveInvitedClient(client->GetNickName());
    std::string responseMessage = buildJoinMsg(client, channel);
    client->AddResponseToBuf(responseMessage);
    return true;
}

bool Executor::Visit(KickRequest *kickRequest) const
{
    const std::string &channelName = kickRequest->GetChannelName();
    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    SharedPtr< Channel > channel = channelRepository->FindByName(channelName);

    SharedPtr< Client > client = kickRequest->GetClient();
    std::string responseMessage;

    std::vector< std::string > targets = kickRequest->GetTargets();
    std::vector< std::string >::iterator iter;

    for (iter = targets.begin(); iter != targets.end(); iter++)
    {
        responseMessage = buildKickoutMsg(client, channelName, *iter, kickRequest->GetMessage());

        channel->BroadcastMessage(responseMessage);
        channel->RemoveClient(*iter);
    }

    if (channel->GetClientCount() == 0)
        channelRepository->RemoveChannel(channelName);

    LOG_TRACE("KickRequest Executed");

    return true;
}

bool Executor::Visit(ModeRequest *modeRequest) const
{
    SharedPtr< Client > client = modeRequest->GetClient();
    const std::string &channelName = modeRequest->GetChannelName();
    const std::string &sign = modeRequest->GetSign();
    const std::string &modeChar = modeRequest->GetModeChar();
    const std::string &modeArgument = modeRequest->GetModeArgument();

    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    SharedPtr< Channel > channel = channelRepository->FindByName(channelName);

    std::string responseMessage;

    if (modeChar == "o")
    {
        ClientRepository *clientRepository = ClientRepository::GetInstance();
        SharedPtr< Client > targetClient = clientRepository->FindByNickName(modeArgument);

        if (sign == "+")
            channel->SetOperator(targetClient);
        else
            channel->RemoveOperator(targetClient->GetNickName());
    }
    else if (modeChar == "l")
    {
        channel->SetClientLimit(atoi(modeArgument.c_str()));

        if (!(sign == "+" && channel->IsClientLimitMode()))
            channel->ToggleClientLimitMode();
    }
    else if (modeChar == "i")
    {
        channel->ToggleInviteOnlyMode();
    }
    else if (modeChar == "k")
    {
        channel->SetKey(modeArgument);
        channel->ToggleKeyMode();
    }
    else if (modeChar == "t")
    {
        channel->ToggleProtectedTopicMode();
    }

    responseMessage = buildModeChangedMsg(client, channelName, sign, modeChar, modeArgument);
    channel->BroadcastMessage(responseMessage);

    LOG_TRACE("ModeRequest Executed");

    return true;
}

bool Executor::Visit(NickRequest *nickRequest) const
{
    ClientRepository *clientRepository = ClientRepository::GetInstance();
    SharedPtr< Client > client = nickRequest->GetClient();

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

        SharedPtr< Channel > channel = client->GetChannel();
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
    SharedPtr< Channel > channel = channelRepository->FindByName(channelName);

    SharedPtr< Client > client = partRequest->GetClient();
    std::string partMessage = buildPartMsg(client, channelName, partRequest->GetReason());

    channel->BroadcastMessage(partMessage);
    channel->RemoveClient(client->GetNickName());
    client->ResetChannel();

    if (channel->GetClientCount() == 0)
        channelRepository->RemoveChannel(channelName);

    LOG_TRACE("PartRequest Executed");

    return true;
}

bool Executor::Visit(PassRequest *passRequest) const
{
    SharedPtr< Client > client = passRequest->GetClient();

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

    SharedPtr< Client > client = privmsgRequest->GetClient();
    std::string privateMessage;

    std::vector< std::string > targets = privmsgRequest->GetTargets();
    std::vector< std::string >::iterator iter;

    for (iter = targets.begin(); iter != targets.end(); iter++)
    {
        privateMessage = buildPrivateMsg(client, *iter, privmsgRequest->GetMessage());

        if (iter->front() == '#')
        {
            SharedPtr< Channel > targetChannel = channelRepository->FindByName(*iter);
            targetChannel->BroadcastMessageExcludingRequestor(privateMessage, client->GetNickName());

            LOG_TRACE("PrivmsgRequest Executing - BroadcastMessage");
        }
        else
        {
            SharedPtr< Client > targetClient = clientRepository->FindByNickName(*iter);
            targetClient->AddResponseToBuf(privateMessage);

            LOG_TRACE("PrivmsgRequest Executing - DirectMessage");
        }
    }

    LOG_TRACE("PrivmsgRequest Executed");

    return true;
}

bool Executor::Visit(QuitRequest *quitRequest) const
{
    SharedPtr< Client > client = quitRequest->GetClient();
    SharedPtr< Channel > channel = client->GetChannel();
    if (channel)
    {
        std::string quitMessage = buildQuitMsg(client, quitRequest->GetReason());

        channel->BroadcastMessageExcludingRequestor(quitMessage, client->GetNickName());
        LOG_DEBUG("QuitRequest Executing");
    }

    disconnectClient(client->GetSocket());

    LOG_TRACE("QuitRequest Executed");

    return true;
}

bool Executor::Visit(TopicRequest *topicRequest) const
{
    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    SharedPtr< Channel > channel = channelRepository->FindByName(topicRequest->GetChannelName());

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
    SharedPtr< Client > client = userRequest->GetClient();

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

std::string Executor::buildNickChangedMsg(SharedPtr< Client > client, const std::string &nickName) const
{
    std::stringstream nickChangedMessage;

    nickChangedMessage << ":" << client->GetClientInfo() << " NICK "
                       << ":" << nickName;

    return nickChangedMessage.str();
}

std::string Executor::buildWelcomeMsg(SharedPtr< Client > client) const
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
                                            const std::string &channelName) const
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream replyMessage;
    replyMessage << ":" << envManager->GetServerName() << " 341 " << nickName << " " << targetNickName << " "
                 << channelName;

    return replyMessage.str();
}

std::string Executor::buildInvitedIntoChannelMsg(const std::string &nickName, const std::string &targetNickName,
                                                 const std::string &channelName) const
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream replyMessage;
    replyMessage << ":" << envManager->GetServerName() << " NOTICE " << channelName << " :*** " << nickName
                 << " invited " << targetNickName << " into the channel";

    return replyMessage.str();
}

std::string Executor::buildInvitationMsg(SharedPtr< Client > client, const std::string &targetNickName,
                                         const std::string &channelName) const
{
    std::stringstream invitationMessage;
    invitationMessage << ":" << client->GetClientInfo() << " INVITE " << targetNickName << " :" << channelName;

    return invitationMessage.str();
}

std::string Executor::buildKickoutMsg(SharedPtr< Client > client, const std::string &channelName,
                                      const std::string &targetNickName, const std::string &message) const
{
    std::stringstream kickoutMessage;
    kickoutMessage << ":" << client->GetClientInfo() << " KICK " << channelName << " " << targetNickName << " "
                   << message;

    return kickoutMessage.str();
}

std::string Executor::buildTopicChangedMsg(SharedPtr< Client > client, const std::string &channelName,
                                           const std::string &topic) const
{
    std::stringstream topicChangedMessage;
    topicChangedMessage << ":" << client->GetClientInfo() << " TOPIC " << channelName << " :" << topic;

    return topicChangedMessage.str();
}

std::string Executor::buildQuitMsg(SharedPtr< Client > client, const std::string &reason) const
{
    std::stringstream quitMessage;
    quitMessage << ":" << client->GetClientInfo() << " Quit :";

    if (reason.empty())
        quitMessage << "Client exited";
    else
        quitMessage << "Quit: " << reason;

    return quitMessage.str();
}

std::string Executor::buildPrivateMsg(SharedPtr< Client > client, const std::string &target,
                                      const std::string &message) const
{
    std::stringstream privateMessage;
    privateMessage << ":" << client->GetClientInfo() << " PRIVMSG " << target << " " << message;

    return privateMessage.str();
}

std::string Executor::buildPongMsg(const std::string &token) const
{
    EnvManager *envManager = EnvManager::GetInstance();
    const std::string &serverName = envManager->GetServerName();

    std::stringstream pongMessage;
    pongMessage << ":" << serverName << " PONG " << serverName << " :" << token;

    return pongMessage.str();
}

std::string Executor::buildPartMsg(SharedPtr< Client > client, const std::string &channelName,
                                   const std::string &reason) const
{
    std::stringstream partMessage;
    partMessage << ":" << client->GetClientInfo() << " PART " << channelName;

    if (!reason.empty())
        partMessage << " " << reason;

    return partMessage.str();
}

std::string Executor::buildModeChangedMsg(SharedPtr< Client > client, const std::string &channelName,
                                          const std::string &sign, const std::string &modeChar,
                                          const std::string &modeArgument) const
{
    std::stringstream modeChangedMessage;
    modeChangedMessage << ":" << client->GetClientInfo() << " MODE " << channelName;

    if (modeArgument.empty())
        modeChangedMessage << " :" << sign << modeChar;
    else
        modeChangedMessage << " " << sign << modeChar << " :" << modeArgument;

    return modeChangedMessage.str();
}

std::string Executor::buildJoinMsg(SharedPtr< Client > client, SharedPtr< Channel > channel) const
{
    EnvManager *envManager = EnvManager::GetInstance();
    const std::string &channelName = channel->GetName();
    const std::string &serverName = envManager->GetServerName();
    const std::string &nickName = client->GetNickName();

    std::stringstream joinMessage;
    joinMessage << ":" << client->GetClientInfo() << " JOIN " << channelName << "\r\n";
    // TODO: 토픽이 없을 때
    if (!channel->GetTopic().empty())
    {
        joinMessage << ":" << serverName << " 332 " << nickName << " " << channelName << " " << channel->GetTopic()
                    << "\r\n";
    }
    joinMessage << ":" << serverName << " 353 " << nickName << " = " << channelName << " :" << channel->GetClientsList()
                << "\r\n";
    joinMessage << ":" << serverName << " 366 " << nickName << " " << channelName << " :End of /Names list.";

    return joinMessage.str();
}