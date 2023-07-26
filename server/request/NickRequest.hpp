#pragma once

#include "Request.hpp"
#include "NickCommand.hpp"

class NickRequest : public Request {

public:
    NickRequest(handle_t socket);

    void ValidateRequest() const;
    Command *BuildCommand() const;

    void SetNickName(const std::string &nickName);

    const std::string &GetNickName() const;

private:
    std::string mNickname;

};