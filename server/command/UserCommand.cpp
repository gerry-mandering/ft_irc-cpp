#include "UserCommand.hpp"

void UserCommand::Execute() const {
    Client *client = mUserRequest->GetClient();

    client->SetUserName(mUserRequest->GetUserName());
    client->SetHostName(mUserRequest->GetHostName());
    client->SetServerName(mUserRequest->GetServerName());
    client->SetRealName(mUserRequest->GetRealName());
    client->SetUserInfoEntered();
}

void UserCommand::setUserRequest(const UserRequest *userRequest) {
    mUserRequest = userRequest;
}
