#pragma once

#include "Request.hpp"
#include "../command/PartCommand.hpp"

class PartRequest : public Request {

public:
    PartRequest(handle_t socket);

    void ValidateRequest() const;
    Command *BuildCommand() const;

    void SetChannelName(const std::string &channelName);
    void SetReason(const std::string &reason);

    const std::string &GetChannelName() const;
    const std::string &GetReason() const;

private:
    std::string mChannelName;
    std::string mReason;

};
