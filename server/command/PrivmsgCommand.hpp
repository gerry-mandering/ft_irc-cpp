#pragma once

#include "Command.hpp"
#include "PrivmsgRequest.hpp"

class PrivmsgCommand : public Command {

public:
    void Execute() const;

    void SetPrivmsgRequest(PrivmsgRequest *privmsgRequest);

private:
    PrivmsgRequest *mPrivmsgRequest;

};