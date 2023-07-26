#pragma once

#include "Command.hpp"
#include "PrivmsgRequest.hpp"

class PrivmsgCommand : public Command {

public:
    void Execute(PrivmsgRequest *privmsgRequest) const;

};