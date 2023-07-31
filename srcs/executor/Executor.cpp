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
    std::string replyInvitingMessage =
        BuildReplyInvitingMsg(client->GetNickName(), inviteRequest->GetNickName(), inviteRequest->GetChannelName());

    // RPL_INVITING 메세지 요청자에게 보내기
    client->addResponseToBuf(replyInvitingMessage);

    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(inviteRequest->GetChannelName());
    std::string InvitedIntoChannelMessage = BuildInvitedIntoChannelMsg(
        client->GetNickName(), inviteRequest->GetNickName(), inviteRequest->GetChannelName());

    // 채널에 속한 클라이언트들에게 초대 알리기
    channel->BroadcastMessageExcludingRequestor(InvitedIntoChannelMessage, client->GetNickName());

    Client *targetClient = clientRepository->FindByNickName(inviteRequest->GetNickName());
    std::string invitationMessage =
        BuildInvitationMsg(client, inviteRequest->GetNickName(), inviteRequest->GetChannelName());

    // 초대를 받은 사람에게 초대장 메세지 보내기
    targetClient->addResponseToBuf(invitationMessage);

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
    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(kickRequest->GetChannelName());

    Client *client = kickRequest->GetClient();
    std::string responseMessage;

    std::vector<std::string> targets = kickRequest->GetTargets();
    std::vector<std::string>::iterator iter;

    for (iter = targets.begin(); iter != targets.end(); iter++)
    {
        responseMessage = BuildKickoutMsg(client, kickRequest->GetChannelName(), *iter, kickRequest->GetMessage());

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
        std::stringstream responseMessage;
        responseMessage << client->GetClientInfo() << "NICK :" << nickRequest->GetNickName();

        Channel *channel = client->GetChannel();
        if (channel)
            channel->BroadcastMessage(responseMessage.str());
        else
            client->addResponseToBuf(responseMessage.str());

        LOG_TRACE("NickRequest Executing - Registered User Changed NickName");
    }

    if (!client->HasRegistered() && client->HasEnteredUserInfo() && client->HasEnteredPassword())
    {
        client->addResponseToBuf(BuildWelcomeMsg(client));
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
    EnvManager *envManager = EnvManager::GetInstance();
    std::string serverName = envManager->GetServerName();

    std::stringstream responseMessage;
    responseMessage << ":" << serverName << " PONG " << serverName << " :" << pingRequest->GetToken();

    Client *client = pingRequest->GetClient();
    client->addResponseToBuf(responseMessage.str());

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
        responseMessage << client->GetClientInfo() << " PRIVMSG " << *iter << privmsgRequest->GetMessage();

        if (iter->front() == '#')
        {
            Channel *targetChannel = channelRepository->FindByName(*iter);
            targetChannel->BroadcastMessage(responseMessage.str());
        }
        else
        {
            Client *targetClient = clientRepository->FindByNickName(*iter);
            targetClient->addResponseToBuf(responseMessage.str());
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

    client->addResponseToBuf(responseMessage.str());

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
        client->addResponseToBuf(BuildWelcomeMsg(client));
        client->SetRegistered();

        LOG_TRACE("UserRequest Executing - SetRegistered");
    }

    LOG_TRACE("UserRequest Executed");

    return true;
}

std::string Executor::BuildWelcomeMsg(Client *client)
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
        << ":" << serverName << " 004 " << client->GetNickName() << " " << serverName << " InspIRCd-3 iklt\r\n"
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

std::string Executor::BuildReplyInvitingMsg(const std::string &nickName, const std::string &targetNickName,
                                            const std::string &channelName)
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream replyMessage;
    replyMessage << ":" << envManager->GetServerName() << " 341 " << nickName << " " << targetNickName << " "
                 << channelName;

    return replyMessage.str();
}

std::string Executor::BuildInvitedIntoChannelMsg(const std::string &nickName, const std::string &targetNickName,
                                                 const std::string &channelName)
{
    EnvManager *envManager = EnvManager::GetInstance();

    std::stringstream replyMessage;
    replyMessage << ":" << envManager->GetServerName() << " NOTICE " << channelName << " :*** " << nickName
                 << " invited " << targetNickName << " into the channel";

    return replyMessage.str();
}

std::string Executor::BuildInvitationMsg(Client *client, const std::string &targetNickName,
                                         const std::string &channelName)
{
    std::stringstream invitationMessage;
    invitationMessage << client->GetClientInfo() << " INVITE " << targetNickName << " :" << channelName;

    return invitationMessage.str();
}

std::string Executor::BuildKickoutMsg(Client *client, const std::string &channelName, const std::string &targetNickName,
                                      const std::string &message)
{
    std::stringstream kickoutMessage;
    kickoutMessage << client->GetClientInfo() << " KICK " << channelName << " " << targetNickName << " :" << message;

    return kickoutMessage.str();
}