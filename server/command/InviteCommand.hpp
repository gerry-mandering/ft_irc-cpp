#pragma once

#include "Command.hpp"
#include "InviteRequest.hpp"

class InviteCommand : public Command {

public:
    void Execute() const;

    void SetInviteRequest(InviteRequest *inviteRequest);

private:
    InviteRequest *mInviteRequest;

};