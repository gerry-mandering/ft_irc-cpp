#pragma once

#include "Request.hpp"
#include "QuitCommand.hpp"

class QuitRequest : public Request {

public:
    QuitRequest(handle_t socket);

    void ValidateRequest() const;
    Command *BuildCommand() const;

    void SetReason(const std::string &reason);

    const std::string &GetReason() const;

private:
    std::string mReason;

};