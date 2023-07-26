#pragma once

#include <vector>
#include "Request.hpp"

class PrivmsgRequest : public Request {

public:
    PrivmsgRequest(handle_t socket);

    void AcceptValidator(Validator *validator);
    void AcceptCommand(Command *command);

    void AddTarget(const std::string &target);
    void SetMessage(const std::string &message);

    const std::vector<std::string> &GetTargets() const;
    const std::string &GetMessage() const;

private:
    std::vector<std::string> mTargets;
    std::string mMessage;

};