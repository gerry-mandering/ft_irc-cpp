#pragma once

#include "ChannelRepository.hpp"
#include "Client.hpp"
#include "ClientRepository.hpp"
#include "EnvManager.hpp"
#include "LoggingHandler.hpp"
#include "RequestConcrete.hpp"
#include "SingletonTemplate.hpp"
#include "VisitorPattern.hpp"
#include <sstream>

class Executor : public TSingleton<Executor>, public visitor_pattern::Visitor
{

  public:
    bool Visit(CapRequest *capRequest) const;
    bool Visit(InviteRequest *inviteRequest) const;
    bool Visit(JoinRequest *joinRequest) const;
    bool Visit(KickRequest *kickRequest) const;
    bool Visit(ModeRequest *modeRequest) const;
    bool Visit(NickRequest *nickRequest) const;
    bool Visit(PartRequest *partRequest) const;
    bool Visit(PassRequest *passRequest) const;
    bool Visit(PingRequest *pingRequest) const;
    bool Visit(PrivmsgRequest *privmsgRequest) const;
    bool Visit(QuitRequest *quitRequest) const;
    bool Visit(TopicRequest *topicRequest) const;
    bool Visit(UserRequest *userRequest) const;

  private:
    static std::string buildNickChangedMsg(Client *client, const std::string &nickName);
    static std::string buildWelcomeMsg(Client *client);
    static std::string buildReplyInvitingMsg(const std::string &nickName, const std::string &targetNickName,
                                             const std::string &channelName);
    static std::string buildInvitedIntoChannelMsg(const std::string &nickName, const std::string &targetNickName,
                                                  const std::string &channelName);
    static std::string buildInvitationMsg(Client *client, const std::string &targetNickName,
                                          const std::string &channelName);
    static std::string buildKickoutMsg(Client *client, const std::string &channelName,
                                       const std::string &targetNickName, const std::string &message);
    static std::string buildTopicChangedMsg(Client *client, const std::string &channelName, const std::string &topic);
    static std::string buildClosingLinkMsg(Client *client, const std::string &reason);
    static std::string buildQuitMsg(Client *client, const std::string &reason);
    static std::string buildPrivateMsg(Client *client, const std::string &target, const std::string &message);
    static std::string buildPongMsg(const std::string &token);
    static std::string buildPartMsg(Client *client, const std::string &channelName, const std::string &reason);
};
