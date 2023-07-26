#pragma once

#include "Request.hpp"
#include "PassCommand.hpp"

class PassRequest : public Request {

public:
    PassRequest(handle_t socket);

    void ValidateRequest() const;
    Command *BuildCommand() const;

    void SetPassword(const std::string &password);

    const std::string &GetPassword() const;

private:
    std::string mPassword;

};