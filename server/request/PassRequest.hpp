#pragma once

#include "Request.hpp"

class PassRequest : public Request {

public:
    PassRequest(handle_t socket);

    void AcceptValidator(Validator *validator);
    void AcceptCommand(Command *command);

    void SetPassword(const std::string &password);

    const std::string &GetPassword() const;

private:
    std::string mPassword;

};