#include "Executor.hpp"
#include "Reactor.hpp"
#include "def.h"

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

// TODO: dahkang 레포에 등록하거나 레포에서 NICK 변경하는 로직이 빠져있는 것 같음.
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