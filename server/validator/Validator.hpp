#pragma once

//TODO enum 필요 없는 코드 일 수도?
typedef enum eErrorType {
    ACCESS_DENIED,
    ERR_ALREADYREGISTRED = 462,
} eErrorType;

#include <sstream>
#include "Client.hpp"

class Validator {

public:
    virtual bool Validate() const = 0;

protected:
    virtual ~Validator() {}
    void AlreadyRegisteredError(const std::string &nickName) const;
    void AccessDeniedError(const std::string &userName, const std::string &hostName) const;

    void SetClient();

private:
    Client *mClient;

};