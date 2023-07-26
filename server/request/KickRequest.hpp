#pragma once

#include <vector>
#include "Request.hpp"

class KickRequest : public Request {

public:
    KickRequest(handle_t socket);

    void AcceptValidator(Validator *validator);
    void AcceptCommand(Command *command);

    void SetNickName(const std::string &nickName);
    void SetMessage(const std::string &message);

    const std::vector<std::string> &GetNickNames() const;
    const std::string &GetMessage() const;

private:
    std::vector<std::string> mNickNames;
    std::string mMessage;

};