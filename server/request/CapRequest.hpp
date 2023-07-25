#pragma once

#include "Request.hpp"
#include "../command/CapCommand.hpp"

class CapRequest : public Request {

public:
    CapRequest(handle_t socket);

    void ValidateRequest() const;
    Command *BuildCommand() const;

    void SetSubCommand(const std::string &subCommand);

    const std::string &GetSubCommand() const;

private:
    std::string mSubCommand;

};