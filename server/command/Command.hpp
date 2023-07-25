#pragma once

class Command {

public:
    virtual void Execute() const = 0;

protected:
    virtual ~Command();

};