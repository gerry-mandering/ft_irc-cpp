#pragma once

class Validator {

public:
    virtual void Validate() const = 0;

protected:
    virtual ~Validator() {}

};