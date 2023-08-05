#pragma once

#include "Channel.hpp"
#include "ChannelRepository.hpp"
#include "Client.hpp"
#include "ClientRepository.hpp"
#include "EnvManager.hpp"
#include "LoggingHandler.hpp"
#include "RequestConcrete.hpp"
#include "SingletonTemplate.hpp"
#include "VisitorPattern.hpp"
#include <cstdlib>
#include <sstream>
#include <string>

class Validator : public TSingleton< Validator >, public visitor_pattern::Visitor
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
    std::string buildAlreadyRegisteredMsg(const std::string &nickName) const;
    std::string buildAccessDeniedMsg(const std::string &userName, const std::string &hostName) const;
    std::string buildNickNameInUseMsg(const std::string &nickName, const std::string &newNickName) const;
    std::string buildNotRegisteredMsg(const std::string &commandType, const std::string &nickName) const;
    std::string buildNoSuchChannelMsg(const std::string &nickName, const std::string &channelName) const;
    std::string buildNoSuchNickMsg(const std::string &nickName, const std::string &targetNickName) const;
    std::string buildNotOnChannelMsg(const std::string &nickName, const std::string &channelName) const;
    std::string buildUserOnChannelMsg(const std::string &nickName, const std::string &targetNickName,
                                      const std::string &channelName) const;
    std::string buildUserNotInChannelMsg(const std::string &nickName, const std::string &targetNickName,
                                         const std::string &channelName) const;
    std::string buildNotChannelOperatorMsg(const std::string &nickName, const std::string &channelName) const;
    std::string buildCannotSendToChannelMsg(const std::string &nickName, const std::string &channelName) const;
    std::string buildKeySetMsg(const std::string &nickName, const std::string &channelName) const;
    std::string buildCannotJoinChannelMsg(const std::string &nickName, const std::string &channelName) const;

    bool validateOperUserMode(SharedPtr< Client > client, SharedPtr< Channel > channel, ModeRequest *modeRequest) const;
    bool validateClientLimitMode(SharedPtr< Channel > channel, ModeRequest *modeRequest) const;
    bool validateInviteOnlyMode(SharedPtr< Channel > channel, ModeRequest *modeRequest) const;
    bool validateKeyMode(SharedPtr< Client > client, SharedPtr< Channel > channel, ModeRequest *modeRequest) const;
    bool validateProtectedTopicMode(SharedPtr< Channel > channel, ModeRequest *modeRequest) const;

    bool inviteModeOK(SharedPtr< Channel > channel, const std::string &nickName) const;
    bool keyModeOK(SharedPtr< Channel > channel, const std::string &key) const;
    bool limitModeOK(SharedPtr< Channel > channel) const;
    bool notAlreadyInChan(SharedPtr< Client > client, SharedPtr< Channel > channel) const;
};
