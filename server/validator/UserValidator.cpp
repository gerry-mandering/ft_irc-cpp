#include "UserValidator.hpp"

bool UserValidator::Validate() const {
    Client *client = mUserRequest->GetClient();

    if (client->EnteredUserInfo()) {
        AlreadyRegisteredError(mUserRequest->GetUserName());
        return false;
    }

    if (client->EnteredNickName() && !client->EnteredPassword()) {
        AccessDeniedError(client->GetNickName());
        return false;
    }

    return true;
}

void UserValidator::SetUserRequest(UserRequest *userRequest) {
    mUserRequest = userRequest;
    mClient = userRequest->GetClient();
}

void UserValidator::HandleError(eErrorType errorType) const {
    Client *client = mUserRequest->GetClient();

    //TODO 원래는 GetHostName()이 아니라 아이피 주소로 출력됨 방법 찾기
    if (errorType == ACCESS_DENIED)
        ErrorMessage << "Error :Closing link: (" << mUserRequest->GetUserName() << "@" << mUserRequest->GetHostName() << ") [Access denied by configuration]";
    else if (errorType == ERR_ALREADYREGISTRED && client->EnteredNickName())
        ErrorMessage << ":irc.local 462 " << client->GetNickName() << ":You may not reregister";
    else
        ErrorMessage << ":irc.local 462 * :You may not reregister";

    client->InsertResponse(ErrorMessage.str());
}
