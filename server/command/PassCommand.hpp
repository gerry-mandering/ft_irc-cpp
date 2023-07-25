#pragma once

#include "Command.hpp"

class PassCommand : public Command {

public:
    void Execute(Request *request) const;

};