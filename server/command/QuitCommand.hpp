#pragma once

#include "Command.hpp"
#include "QuitRequest.hpp"

class QuitCommand : public Command {

public:
    void Execute() const;

    void setQuitRequest(QuitRequest *quitRequest);

private:
    QuitRequest *mQuitRequest;

};