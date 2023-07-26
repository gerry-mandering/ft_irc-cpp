#include "NickCommand.hpp"

void NickCommand::Execute() const {}

void NickCommand::setNickRequest(NickRequest *nickRequest) {
    mNickRequest = nickRequest;
}
