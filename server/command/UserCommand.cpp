#include "UserCommand.hpp"

void UserCommand::Execute() const {
    Client *client = mUserRequest->GetClient();

    client->SetUserName(mUserRequest->GetUserName());
    client->SetHostName(mUserRequest->GetHostName());
    client->SetServerName(mUserRequest->GetServerName());
    client->SetRealName(mUserRequest->GetRealName());
    client->SetUserInfoEntered();

    //NICK PASS 들어와 있으면 registered flag 켜주기
}

void UserCommand::SetUserRequest(const UserRequest *userRequest) {
    mUserRequest = userRequest;
}
