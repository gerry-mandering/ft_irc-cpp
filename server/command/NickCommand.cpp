#include "NickCommand.hpp"

void NickCommand::Execute(NickRequest *nickRequest) const {
    Client *client = nickRequest->GetClient();

    client->SetNickName(nickRequest->GetNickName());
    client->SetNickNameEntered();
}
