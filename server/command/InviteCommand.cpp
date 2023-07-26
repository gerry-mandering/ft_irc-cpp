#include "InviteCommand.hpp"

void InviteCommand::Execute() const {}

void InviteCommand::SetInviteRequest(InviteRequest *inviteRequest) {
    mInviteRequest = inviteRequest;
}
