#pragma once

#include "Request.hpp"

class PartRequest : public Request
{

  public:
    PartRequest(handle_t socket, const std::string &channelName, const std::string &reason);

    bool Accept(visitor_pattern::Visitor *visitor);

    void SetChannelName(const std::string &channelName);
    void SetReason(const std::string &reason);

    const std::string &GetChannelName() const;
    const std::string &GetReason() const;

    friend std::ostream &operator<<(std::ostream &os, const PartRequest &partRequest);

  private:
    std::string mChannelName;
    std::string mReason; // TODO 생성시 std::string()으로 빈 문자열 넣어주기 Executer 구현시 빈
                         // 문자열 비교 사용함
};
