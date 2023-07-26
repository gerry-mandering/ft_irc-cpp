#pragma once

#include "Command.hpp"
#include "JoinRequest.hpp"

class JoinCommand : public Command {

public:
    void Execute(JoinRequest *joinRequest) const;

};