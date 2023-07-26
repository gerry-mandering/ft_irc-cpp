#pragma once

#include "Command.hpp"
#include "QuitRequest.hpp"

class QuitCommand : public Command {

public:
    void Execute(QuitRequest *quitRequest) const;

};