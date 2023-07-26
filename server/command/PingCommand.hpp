#pragma once

#include "Command.hpp"
#include "PingRequest.hpp"

class PingCommand : public Command {

public:
    void Execute(PingRequest *pingRequest) const;

};