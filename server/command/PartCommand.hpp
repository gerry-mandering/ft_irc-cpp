#pragma once

#include "Command.hpp"
#include "PartRequest.hpp"

class PartCommand : public Command {

public:
    void Execute(PartRequest *partRequest) const;

};