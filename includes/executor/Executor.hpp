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
    static std::string BuildReplyInvitingMsg(const std::string &nickName, const std::string &targetNickName,
                                             const std::string &channelName);
    static std::string BuildInvitedIntoChannelMsg(const std::string &nickName, const std::string &targetNickName,
                                                  const std::string &channelName);
    static std::string BuildInvitationMsg(Client *client, const std::string &targetNickName,
                                          const std::string &channelName);
    static std::string BuildKickoutMsg(Client *client, const std::string &channelName,
                                       const std::string &targetNickName, const std::string &message);
};
