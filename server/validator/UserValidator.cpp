#include "UserValidator.hpp"

void UserValidator::Validate() const {
    Client *client = mUserRequest->GetClient();
    // USER, PASS 아무것도 안 들어온 경우

    // USER만 들어온 경우 PASS를 입력하지 않고 register를 시도해서 Error :Closing link: (<username>@<hostname>) [Access denied by configuration]

    // PASS만 들어온 경우

    // USER, PASS 둘 다 들어온 경우

    // 이미 registered 되어 있는 경우에는 :irc.local 462 minseok2 :You may not reregister
    client->
}

void UserValidator::SetUserRequest(UserRequest *userRequest) {
    mUserRequest = userRequest;
}
