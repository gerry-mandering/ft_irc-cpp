#include "PingCommand.hpp"

void PingCommand::Execute() const {}

void PingCommand::setPingRequest(PingRequest *pingRequest) {
    mPingRequest = pingRequest;
}
