#pragma once

#include "CapRequest.hpp"
#include "Client.hpp"
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

class Executor : public TSingleton<Executor>, visitor_pattern::Visitor
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
};
