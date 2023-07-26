#include "UserValidator.hpp"

void UserValidator::Validate() const {
    Client *client = mUserRequest->GetClient();

    if (client->EnteredUserInfo())
        HandleError(ERR_ALREADYREGISTRED);
    else if (client->EnteredNickName() && !client->EnteredPassword())
        HandleError(CLOSING_LINK);

    client->SetUserName(mUserRequest->GetUserName());
    client->SetHostName(mUserRequest->GetHostName());
    client->SetServerName(mUserRequest->GetServerName());
    client->SetRealName(mUserRequest->GetRealName());
    client->SetUserInfoEntered();

    if (client->EnteredNickName() && client->EnteredPassword())
        client->SetRegistered();
}

void UserValidator::SetUserRequest(UserRequest *userRequest) {
    mUserRequest = userRequest;
}

void UserValidator::HandleError(eErrorType errorType) const {
    Client *client = mUserRequest->GetClient();
    std::stringstream ErrorMessage;

    if (errorType == ERR_ALREADYREGISTRED) {
        if (client->EnteredNickName())
            ErrorMessage << ":irc.local 462 " << client->GetNickName() << ":You may not reregister";
        else
            ErrorMessage << ":irc.local 462 * :You may not reregister";
    } else if (errorType == CLOSING_LINK)
        ErrorMessage << "Error :Closing link: (" << mUserRequest->GetUserName() << "@" << mUserRequest->GetHostName() << ") [Access denied by configuration]";
    //TODO 원래는 GetHostName()이 아니라 아이피 주소로 출력됨 방법 찾기

    client->InsertResponse(ErrorMessage.str());
}
