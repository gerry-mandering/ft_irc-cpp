#pragma once

#include "Command.hpp"
#include "CapRequest.hpp"

class CapCommand : public Command {

public:
    void Execute(CapRequest *capRequest) const;

};