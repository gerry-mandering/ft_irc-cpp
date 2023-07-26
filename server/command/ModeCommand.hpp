#pragma once

#include "Command.hpp"
#include "ModeRequest.hpp"

class ModeCommand : public Command {

public:
    void Execute() const;

    void setModeRequest(ModeRequest *modeRequest);

private:
    ModeRequest *mModeRequest;

};
