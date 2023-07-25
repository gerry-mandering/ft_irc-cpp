#pragma once

#include "Request.hpp"

class NickRequest : public Request {

public:
    NickRequest(handle_t socket);

    void SetNickName(const std::string &nickName);

    const std::string &GetNickName() const;

private:
    std::string mNickname;

};