#include "PingCommand.hpp"

void PingCommand::Execute() const {}

void PingCommand::SetPingRequest(PingRequest *pingRequest) {
    mPingRequest = pingRequest;
}
