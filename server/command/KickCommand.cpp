#include "KickCommand.hpp"

void KickCommand::Execute() const {}

void KickCommand::setKickRequest(KickRequest *kickRequest) {
    mKickRequest = kickRequest;
}
