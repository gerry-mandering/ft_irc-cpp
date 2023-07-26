#include "NickCommand.hpp"

void NickCommand::Execute() const {
    Client *client = mNickRequest->GetClient();

    client->SetNickName(mNickRequest->GetNickName());
    client->SetNickNameEntered();
}

void NickCommand::SetNickRequest(NickRequest *nickRequest) {
    mNickRequest = nickRequest;
}
