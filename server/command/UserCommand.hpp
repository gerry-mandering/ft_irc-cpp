#pragma once

#include "Command.hpp"
#include "../request/UserRequest.hpp"

class UserCommand : public Command {

public:
    UserCommand(UserRequest *userRequest);

    void Execute() const;

private:
    UserRequest *mUserRequest;

};