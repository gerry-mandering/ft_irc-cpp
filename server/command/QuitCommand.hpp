#pragma once

#include "Command.hpp"
#include "QuitRequest.hpp"

class QuitComamnd : public Command {

public:
    void Execute(QuitRequest *quitRequest) const;

};