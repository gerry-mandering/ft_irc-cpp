#include "NickRequest.hpp"

NickRequest::NickRequest(handle_t socket) : Request(socket) {}

void NickRequest::SetNickName(const std::string &nickName) {
    mNickname = nickName;
}

const std::string &NickRequest::GetNickName() const {
    return mNickname;
}