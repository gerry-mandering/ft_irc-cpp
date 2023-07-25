#pragma once

#include <vector>
#include "Request.hpp"
#include "../command/KickCommand.hpp"

class KickRequest : public Request {

public:
    KickRequest(handle_t socket);

    void ValidateRequest() const;
    Command *BuildCommand() const;

    void SetNickName(const std::string &nickName);
    void SetMessage(const std::string &message);

    const std::vector<std::string> &GetNickNames() const;
    const std::string &GetMessage() const;

private:
    std::vector<std::string> mNickNames;
    std::string mMessage;

};