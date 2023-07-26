#include "InviteCommand.hpp"

void InviteCommand::Execute() const {}

void InviteCommand::setInviteRequest(InviteRequest *inviteRequest) {
    mInviteRequest = inviteRequest;
}
