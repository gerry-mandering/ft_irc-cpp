#pragma once

#include "Request.hpp"

class NickRequest : public Request
{

  public:
    NickRequest(handle_t socket, const std::string &nickname);

    bool Accept(visitor_pattern::Visitor *visitor);

    void SetNickName(const std::string &nickName);

    const std::string &GetNickName() const;

  private:
    std::string mNickname;
};