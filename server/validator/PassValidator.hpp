#pragma once

#include "Validator.hpp"
#include "PassRequest.hpp"

class PassValidator : public Validator {

public:
    bool Validate() const;

    void SetPassRequest(PassRequest *passRequest);

private:
    PassRequest *mPassRequest;

};