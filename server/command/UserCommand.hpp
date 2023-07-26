#pragma once

#include "Command.hpp"
#include "UserRequest.hpp"

class UserCommand : public Command {

public:
    void Execute() const;

    void setUserRequest(const UserRequest *userRequest);

private:
    const UserRequest *mUserRequest;

};