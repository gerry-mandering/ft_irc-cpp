#include "Validator.hpp"

void Validator::AlreadyRegisteredError(const std::string &nickName) const {
    std::stringstream ErrorMessage;
}

void Validator::AccessDeniedError(const std::string &userName, const std::string &hostName) const {
    std::stringstream ErrorMessage;

    ErrorMessage << "Error :Closing link: (" << userName << "@" << hostName << ") [Access denied by configuration]";
    mClient->
}