#include "QuitRequest.hpp"

QuitRequest::QuitRequest(handle_t socket, const std::string &reason) : Request(socket), mReason(reason)
{
    LOG_TRACE("PrivmsgRequest constructor: " << *this);
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

std::ostream &operator<<(std::ostream &os, const QuitRequest &quitRequest)
{
    os << "QuitRequest - Reason: " << quitRequest.mReason;

    return os;
}
