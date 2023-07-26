#pragma once

#include "Command.hpp"
#include "UserRequest.hpp"

class UserCommand : public Command {

public:
    void Execute() const;

    void SetUserRequest(const UserRequest *userRequest);

private:
    const UserRequest *mUserRequest;

};