#pragma once

#include "Request.hpp"

class PassRequest : public Request
{

  public:
    PassRequest(handle_t socket, const std::string &password);

    bool Accept(visitor_pattern::Visitor *visitor);

    void SetPassword(const std::string &password);

    const std::string &GetPassword() const;

    friend std::ostream &operator<<(std::ostream &os, const PassRequest &passRequest);

  private:
    std::string mPassword;
};