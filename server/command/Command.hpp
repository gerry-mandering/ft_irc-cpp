#pragma once

#include "Request.hpp"

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

class Command {

public:
    //ClearCommands() 만들기
    static Command *GetCommand(eRequestType requestType);

    virtual void Execute(CapRequest *capRequest) const = 0;
    virtual void Execute(InviteRequest *inviteRequest) const = 0;
    virtual void Execute(JoinRequest *joinRequest) const = 0;
    virtual void Execute(KickRequest *kickRequest) const = 0;
    virtual void Execute(ModeRequest *modeRequest) const = 0;
    virtual void Execute(NickRequest *nickRequest) const = 0;
    virtual void Execute(PartRequest *partRequest) const = 0;
    virtual void Execute(PassRequest *passRequest) const = 0;
    virtual void Execute(PingRequest *pingRequest) const = 0;
    virtual void Execute(PrivmsgRequest *privmsgRequest) const = 0;
    virtual void Execute(QuitRequest *quitRequest) const = 0;
    virtual void Execute(TopicRequest *topicRequest) const = 0;
    virtual void Execute(UserRequest *userRequest) const = 0;

protected:
    virtual ~Command() {}

private:
    static const int NUMBER_OF_COMMANDS = 13;
    static Command *mCommands[Command::NUMBER_OF_COMMANDS];

};
