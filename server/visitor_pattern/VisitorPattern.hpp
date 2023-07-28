#pragma once

class CapRequest;
class InviteRequest;
class JoinRequest;
class KickRequest;
class ModeRequest;
class NickRequest;
class PartRequest;
class PassRequest;
class PingRequest;
class PrivmsgRequest;
class QuitRequest;
class TopicRequest;
class UserRequest;

namespace visitor_pattern
{

class Visitor
{

  public:
    virtual bool Visit(CapRequest *capRequest) const = 0;
    virtual bool Visit(InviteRequest *inviteRequest) const = 0;
    virtual bool Visit(JoinRequest *joinRequest) const = 0;
    virtual bool Visit(KickRequest *kickRequest) const = 0;
    virtual bool Visit(ModeRequest *modeRequest) const = 0;
    virtual bool Visit(NickRequest *nickRequest) const = 0;
    virtual bool Visit(PartRequest *partRequest) const = 0;
    virtual bool Visit(PassRequest *passRequest) const = 0;
    virtual bool Visit(PingRequest *pingRequest) const = 0;
    virtual bool Visit(PrivmsgRequest *privmsgRequest) const = 0;
    virtual bool Visit(QuitRequest *quitRequest) const = 0;
    virtual bool Visit(TopicRequest *topicRequest) const = 0;
    virtual bool Visit(UserRequest *userRequest) const = 0;
};

} // namespace visitor_pattern