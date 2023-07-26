#pragma once

#include "Command.hpp"
#include "ModeRequest.hpp"

class ModeCommand : public Command {

public:
    void Execute(ModeRequest *modeRequest) const;

};