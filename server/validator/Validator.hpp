#pragma once

typedef enum eErrorType {
    CLOSING_LINK,
    ERR_ALREADYREGISTRED = 462,
} eErrorType;

#include <sstream>

class Validator {

public:
    virtual void Validate() const = 0;

protected:
    virtual ~Validator() {}

private:
    virtual void HandleError() const = 0;

};