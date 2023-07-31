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
    client->InsertResponse(replyInvitingMessage);

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
    targetClient->InsertResponse(invitationMessage);

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

        // TODO: 연결시켜보려고 welcomeMessage 이겆저것 해봄...

        // :irc.local NOTICE * :*** Looking up your hostname...
        // 127.000.000.001.57268-127.000.000.001.06667: PING irc.local^M$
        // :irc.local 376 dah :End of message of the day.
        // :dah!root@127.0.0.1 MODE dah :+i
        // 127.000.000.001.06667-127.000.000.001.46614: :irc.local NOTICE dah :*** Could not resolve your hostname:
        // Request timed out; using your IP address (127.0.0.1) instead. :irc.local 001 dah :Welcome to the Localnet IRC
        // Network dah!root@127.0.0.1 :irc.local 002 dah :Your host is irc.local, running version InspIRCd-3 :irc.local
        // 003 dah :This server was created 11:01:07 Jul 29 2023 :irc.local 004 dah irc.local InspIRCd-3 iosw
        // biklmnopstv :bklov :irc.local 005 dah AWAYLEN=200 CASEMAPPING=rfc1459 CHANLIMIT=#:20 CHANMODES=b,k,l,imnpst
        // CHANNELLEN=64 CHANTYPES=# ELIST=CMNTU HOSTLEN=64 KEYLEN=32 KICKLEN=255 LINELEN=512 MAXLIST=b:100 :are
        // supported by this server

        // welcomeMessage << "irc.local" << " NOTICE " << client->GetNickName()
        //                << " :*** Looking up your hostname...\r\n"
        //                << "irc.local" << " 001 " << client->GetNickName() << " :Welcome\r\n"
        //                << "irc.local" << " 002 " << client->GetNickName() << " :Your host is "
        //                << "irc.local" << "\r\n"
        //                << "irc.local" << " 003 " << client->GetNickName()
        //                << " :This server was created 11:01:07 Jul 29 2023\r\n"
        //                << "irc.local" << " 004 " << client->GetNickName() << " "
        //                << "irc.local" << " InspIRCd-3 iosw biklmnopstv :bklov\r\n"
        //                << "irc.local" << " 005 " << client->GetNickName() << " tmp\r\n"
        //                << "irc.local" << " 376 " << client->GetNickName()
        //                << " :End of message of the day.\r\n";

        //    << client->GetNickName() << "!" << "root@127.0.0.1 "
        //    << "MODE " << client->GetNickName() << " :+i\r\n";

        // 기존 welcome message
        welcomeMessage << envManager->GetServerName() << " 001 " << client->GetNickName()
                       << " :Welcome to the PingPong IRC Network " << client->GetClientInfo();

        Reactor &reactor = g_reactor();

        client->InsertResponse(welcomeMessage.str());
        reactor.registerEvent(reactor.getHandler(client->GetSocket()), WRITE_EVENT);

        client->SetRegistered();

        LOG_TRACE("UserRequest Executing - SetRegistered");
    }

    LOG_TRACE("UserRequest Executed");

    return true;
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