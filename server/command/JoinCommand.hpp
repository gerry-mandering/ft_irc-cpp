#pragma once

#include "Command.hpp"
#include "JoinRequest.hpp"

class JoinCommand : public Command {

public:
    void Execute() const;

    void setJoinRequest(JoinRequest *joinRequest);

private:
    JoinRequest *mJoinRequest;

};