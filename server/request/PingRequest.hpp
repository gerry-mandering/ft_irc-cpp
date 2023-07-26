#pragma once

#include "Request.hpp"

class PingRequest : public Request {

public:
    PingRequest(handle_t socket);

    void AcceptValidator(Validator *validator);
    void AcceptCommand(Command *command);

    void SetToken(const std::string &token);

    const std::string &GetToken() const;

private:
    std::string mToken;

};