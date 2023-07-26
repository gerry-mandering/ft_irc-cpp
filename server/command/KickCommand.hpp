#pragma once

#include "Command.hpp"
#include "KickRequest.hpp"

class KickCommand : public Command {

public:
    void Execute(KickRequest *kickRequest) const;

};