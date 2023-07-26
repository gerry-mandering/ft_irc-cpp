#pragma once

#include "Command.hpp"
#include "PassRequest.hpp"

class PassCommand : public Command {

public:
    void Execute() const;

    void setPassRequest(PassRequest *passRequest);

private:
    PassRequest *mPassRequest;

};