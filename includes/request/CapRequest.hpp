#pragma once

#include "Request.hpp"

class CapRequest : public Request
{

  public:
    CapRequest(handle_t socket, const std::string &subCommand);

    bool Accept(visitor_pattern::Visitor *visitor);

    void SetSubCommand(const std::string &subCommand);

    const std::string &GetSubCommand() const;

    friend std::ostream &operator<<(std::ostream &os, const CapRequest &capRequest);

  private:
    std::string mSubCommand;
};