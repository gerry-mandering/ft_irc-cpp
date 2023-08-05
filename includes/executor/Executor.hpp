#pragma once

#include "ChannelRepository.hpp"
#include "Client.hpp"
#include "ClientRepository.hpp"
#include "EnvManager.hpp"
#include "LoggingHandler.hpp"
#include "RequestConcrete.hpp"
#include "SharedPtr.hpp"
#include "SingletonTemplate.hpp"
#include "VisitorPattern.hpp"
#include <cstdlib>
#include <sstream>

class Executor : public TSingleton< Executor >, public visitor_pattern::Visitor
{

  public:
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
    std::string buildNickChangedMsg(SharedPtr< Client > client, const std::string &nickName) const;
    std::string buildWelcomeMsg(SharedPtr< Client > client) const;
    std::string buildReplyInvitingMsg(const std::string &nickName, const std::string &targetNickName,
                                      const std::string &channelName) const;
    std::string buildInvitedIntoChannelMsg(const std::string &nickName, const std::string &targetNickName,
                                           const std::string &channelName) const;
    std::string buildInvitationMsg(SharedPtr< Client > client, const std::string &targetNickName,
                                   const std::string &channelName) const;
    std::string buildKickoutMsg(SharedPtr< Client > client, const std::string &channelName,
                                const std::string &targetNickName, const std::string &message) const;
    std::string buildTopicChangedMsg(SharedPtr< Client > client, const std::string &channelName,
                                     const std::string &topic) const;
    // std::string buildClosingLinkMsg(SharedPtr< Client > client, const std::string &reason) const;
    std::string buildQuitMsg(SharedPtr< Client > client, const std::string &reason) const;
    std::string buildPrivateMsg(SharedPtr< Client > client, const std::string &target,
                                const std::string &message) const;
    std::string buildPongMsg(const std::string &token) const;
    std::string buildPartMsg(SharedPtr< Client > client, const std::string &channelName,
                             const std::string &reason) const;
    std::string buildModeChangedMsg(SharedPtr< Client > client, const std::string &channelName, const std::string &sign,
                                    const std::string &modeChar, const std::string &modeArgument) const;
    std::string buildJoinMsg(SharedPtr< Client > client, SharedPtr< Channel > channel) const;
};
