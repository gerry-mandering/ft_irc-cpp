#pragma once

#include "Command.hpp"
#include "KickRequest.hpp"

class KickCommand : public Command {

public:
    void Execute() const;

    void SetKickRequest(KickRequest *kickRequest);

private:
    KickRequest *mKickRequest;

};