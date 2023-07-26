#pragma once

#include "Command.hpp"
#include "NickRequest.hpp"

class NickCommand : public Command {

public:
    void Execute() const;

    void SetNickRequest(NickRequest *nickRequest);

private:
    NickRequest *mNickRequest;

};