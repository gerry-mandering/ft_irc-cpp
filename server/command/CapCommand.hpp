#pragma once

#include "Command.hpp"
#include "CapRequest.hpp"

class CapCommand : public Command {

public:
    void Execute() const;

    void setCapRequest(CapRequest *capRequest);

private:
    CapRequest *mCapRequest;

};