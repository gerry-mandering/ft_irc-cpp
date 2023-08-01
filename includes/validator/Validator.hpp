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

namespace ft_validator
{
bool keyModeOK(Channel *channel, const std::string &key);
bool limitModeOK(Channel *channel);
bool notAlreadyInChan(Client *client, Channel *channel);
} // namespace ft_validator

class Validator : public TSingleton<Validator>, public visitor_pattern::Visitor
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
    static std::string buildAlreadyRegisteredMsg(const std::string &nickName = "*");
    static std::string buildAccessDeniedMsg(const std::string &userName = "*", const std::string &hostName = "*");
    static std::string buildNickNameInUseMsg(const std::string &newNickName, const std::string &clientNickName = "*");
    static std::string buildNotRegisteredMsg(const std::string &commandType, const std::string &nickName = "*");
    static std::string buildNoSuchChannelMsg(const std::string &nickName, const std::string &channelName);
    static std::string buildNoSuchNickMsg(const std::string &nickName, const std::string &targetNickName);
    static std::string buildNotOnChannelMsg(const std::string &nickName, const std::string &channelName);
    static std::string buildUserOnChannelMsg(const std::string &nickName, const std::string &targetNickName,
                                             const std::string &channelName);
    static std::string buildUserNotInChannelMsg(const std::string &nickName, const std::string &targetNickName,
                                                const std::string &channelName);
    static std::string buildNotChannelOperatorMsg(const std::string &nickName, const std::string &channelName);
    static std::string buildCannotSendToChannelMsg(const std::string &nickName, const std::string &channelName);
    static std::string buildKeySetMsg(const std::string &nickName, const std::string &channelName);
};
