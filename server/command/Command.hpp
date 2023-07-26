#pragma once

#include "Request.hpp"


class Command {

public:
    //ClearCommands() 만들기
    static Command *GetCommand(eRequestType requestType);
    virtual void Execute() const = 0;

protected:
    virtual ~Command() {}

private:
    static const int NUMBER_OF_COMMANDS = 13;
    static Command *mCommands[Command::NUMBER_OF_COMMANDS];

};
