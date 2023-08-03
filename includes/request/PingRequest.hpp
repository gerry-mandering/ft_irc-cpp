#pragma once

#include "Request.hpp"

class PingRequest : public Request
{

  public:
    PingRequest(handle_t socket, const std::string &token);

    bool Accept(visitor_pattern::Visitor *visitor);

    void SetToken(const std::string &token);

    const std::string &GetToken() const;

    friend std::stringstream &operator<<(std::stringstream &ss, const PingRequest &pingRequest);

  private:
    std::string mToken;
};