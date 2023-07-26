#include "JoinCommand.hpp"

void JoinCommand::Execute() const {}

void JoinCommand::SetJoinRequest(JoinRequest *joinRequest) {
    mJoinRequest = joinRequest;
}
