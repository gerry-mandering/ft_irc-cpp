#pragma once

#include "Request.hpp"

class PassRequest : public Request
{

  public:
    PassRequest(handle_t socket);

    void Accept(visitor_pattern::Visitor *visitor);

    void SetPassword(const std::string &password);

    const std::string &GetPassword() const;

  private:
    std::string mPassword;
};