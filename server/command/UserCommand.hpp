#pragma once

#include "Command.hpp"
#include "UserRequest.hpp"

class UserCommand : public Command {

public:
    void Execute(UserRequest *userRequest) const;

};