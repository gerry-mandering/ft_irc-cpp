#include "QuitRequest.hpp"

QuitRequest::QuitRequest(handle_t socket) : Request(socket) {}

void QuitRequest::SetReason(const std::string &reason) {
    mReason = reason;
}

const std::string &QuitRequest::GetReason() const {
    return mReason;
}