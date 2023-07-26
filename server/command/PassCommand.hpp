#pragma once

#include "Command.hpp"
#include "PassRequest.hpp"

class PassCommand : public Command {

public:
    void Execute(PassRequest *passRequest) const;

};