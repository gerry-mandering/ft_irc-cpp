#pragma once

#include "Request.hpp"
#include "PingCommand.hpp"

class PingRequest : public Request {

public:
    PingRequest(handle_t socket);

    void ValidateRequest() const;
    Command *BuildCommand() const;

    void SetToken(const std::string &token);

    const std::string &GetToken() const;

private:
    std::string mToken;

};