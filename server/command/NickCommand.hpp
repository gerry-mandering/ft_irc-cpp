#pragma once

#include "Command.hpp"
#include "NickRequest.hpp"

class NickCommand : public Command {

public:
    void Execute(NickRequest *nickRequest) const;

};