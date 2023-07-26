#pragma once

#include "Command.hpp"
#include "PrivmsgRequest.hpp"

class PrivmsgCommand : public Command {

public:
    void Execute() const;

    void setPrivmsgRequest(PrivmsgRequest *privmsgRequest);

private:
    PrivmsgRequest *mPrivmsgRequest;

};