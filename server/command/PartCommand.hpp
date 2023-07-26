#pragma once

#include "Command.hpp"
#include "PartRequest.hpp"

class PartCommand : public Command {

public:
    void Execute() const;

    void setPartRequest(PartRequest *partRequest);

private:
    PartRequest *mPartRequest;

};