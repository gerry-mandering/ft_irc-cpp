#pragma once

#include "Command.hpp"
#include "KickRequest.hpp"

class KickCommand : public Command {

public:
    void Execute() const;

    void setKickRequest(KickRequest *kickRequest);

private:
    KickRequest *mKickRequest;

};