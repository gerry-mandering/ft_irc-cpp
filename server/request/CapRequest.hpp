#pragma once

#include "Request.hpp"

class CapRequest : public Request
{

  public:
    CapRequest(handle_t socket);

    void Accept(visitor_pattern::Visitor *visitor);

    void SetSubCommand(const std::string &subCommand);

    const std::string &GetSubCommand() const;

  private:
    std::string mSubCommand;
};