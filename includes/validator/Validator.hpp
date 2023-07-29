#pragma once

#include "CapRequest.hpp"
#include "Channel.hpp"
#include "ChannelRepository.hpp"
#include "Client.hpp"
#include "ClientRepository.hpp"
#include "EnvManager.hpp"
#include "InviteRequest.hpp"
#include "JoinRequest.hpp"
#include "KickRequest.hpp"
#include "ModeRequest.hpp"
#include "NickRequest.hpp"
#include "PartRequest.hpp"
#include "PassRequest.hpp"
#include "PingRequest.hpp"
#include "PrivmsgRequest.hpp"
#include "QuitRequest.hpp"
#include "SingletonTemplate.hpp"
#include "TopicRequest.hpp"
#include "UserRequest.hpp"
#include "VisitorPattern.hpp"
#include <sstream>

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
    static std::string BuildAlreadyRegisteredMsg(const std::string &nickName = "*");
    static std::string BuildAccessDeniedMsg(const std::string &userName = "*", const std::string &hostName = "*");
    static std::string BuildNickNameInUseMsg(const std::string &newNickName, const std::string &clientNickName = "*");
    static std::string BuildNotRegisteredMsg(const std::string &commandType, const std::string &nickName = "*");
    static std::string BuildNoSuchChannelMsg(const std::string &nickName, const std::string &channelName);
    static std::string BuildNoSuchNickMsg(const std::string &nickName, const std::string &targetNickName);
    static std::string BuildNotOnChannelMsg(const std::string &nickName, const std::string &channelName);
    static std::string BuildNotChannelOperatorMsg(const std::string &nickName, const std::string &channelName);
    static std::string BuildCannotSendToChannelMsg(const std::string &nickName, const std::string &channelName);
};
