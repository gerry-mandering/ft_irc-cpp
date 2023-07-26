#pragma once

#include "Command.hpp"
#include "InviteRequest.hpp"

class InviteCommand : public Command {

public:
    void Execute(InviteRequest *inviteRequest) const;

};