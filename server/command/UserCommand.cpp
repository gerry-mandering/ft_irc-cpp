#include "UserCommand.hpp"

UserCommand::UserCommand(UserRequest *userRequest) : mUserRequest(userRequest) {}

void UserCommand::Execute() const {
    Client *client = mUserRequest->GetClient();

    client->SetUserName(mUserRequest->GetUserName());
    client->SetHostName(mUserRequest->GetHostName());
    client->SetServerName(mUserRequest->GetServerName());
    client->SetRealName(mUserRequest->GetRealName());
    client->SetUserInfoEntered();
}
