#pragma once

#include "Validator.hpp"
#include "UserRequest.hpp"

class UserValidator : public Validator {

public:
    bool Validate() const;

    void SetUserRequest(UserRequest *userRequest);

private:
    UserRequest *mUserRequest;

};