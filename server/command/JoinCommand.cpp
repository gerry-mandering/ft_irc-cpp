#include "JoinCommand.hpp"

void JoinCommand::Execute() const {}

void JoinCommand::setJoinRequest(JoinRequest *joinRequest) {
    mJoinRequest = joinRequest;
}
