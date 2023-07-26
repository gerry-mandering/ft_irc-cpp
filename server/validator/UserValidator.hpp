#pragma once

#include "Validator.hpp"
#include "UserRequest.hpp"

class UserValidator : public Validator {

public:
    bool Validate(UserRequest *userRequest) const;

};