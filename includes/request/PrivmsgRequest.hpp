#pragma once

#include "Request.hpp"
#include <vector>

class PrivmsgRequest : public Request
{

  public:
    PrivmsgRequest(handle_t socket, const std::vector<std::string> &targets, const std::string &message);

    bool Accept(visitor_pattern::Visitor *visitor);

    void AddTarget(const std::string &target);
    void SetMessage(const std::string &message);

    std::vector<std::string> &GetTargets();
    const std::string &GetMessage() const;

  private:
    std::vector<std::string> mTargets;
    std::string mMessage;
};