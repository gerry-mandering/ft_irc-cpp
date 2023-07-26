#include "KickCommand.hpp"

void KickCommand::Execute() const {}

void KickCommand::SetKickRequest(KickRequest *kickRequest) {
    mKickRequest = kickRequest;
}
