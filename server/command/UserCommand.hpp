#pragma once

#include "Command.hpp"
#include "UserRequest.hpp"

class UserCommand : public Command {

public:
    UserCommand(UserRequest *userRequest);

    void SetRequest();

    void Execute() const;

private:
    UserRequest *mUserRequest;

};