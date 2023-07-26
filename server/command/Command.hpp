#pragma once

#include "../request/Request.hpp"

class Command {

public:
    virtual void Execute() const = 0;

protected:
    virtual ~Command();

};
