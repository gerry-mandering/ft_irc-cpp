#pragma once

#include "Command.hpp"
#include "PingRequest.hpp"

class PingCommand : public Command {

public:
    void Execute() const;

    void SetPingRequest(PingRequest *pingRequest);

private:
    PingRequest *mPingRequest;

};