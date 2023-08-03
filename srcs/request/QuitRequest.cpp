#include "QuitRequest.hpp"

QuitRequest::QuitRequest(handle_t socket, const std::string &reason) : Request(socket), mReason(reason)
{
    LOG_TRACE("PrivmsgRequest constructor called | " << *this);
}

bool QuitRequest::Accept(visitor_pattern::Visitor *visitor)
{
    return visitor->Visit(this);
}

void QuitRequest::SetReason(const std::string &reason)
{
    mReason = reason;
}

const std::string &QuitRequest::GetReason() const
{
    return mReason;
}

std::stringstream &operator<<(std::stringstream &ss, const QuitRequest &quitRequest)
{
    ss << "QuitRequest = { Reason: " << quitRequest.mReason << " }";

    return ss;
}
