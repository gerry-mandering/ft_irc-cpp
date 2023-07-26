#include "PassValidator.hpp"

//TODO PASSWORD EnvManager 통해서 받아오기
#define PASSWORD "1234"

//PASS 잘못 들어온 순간 바로 나가리 처리 - 원래는 registered 할때 검증
bool PassValidator::Validate() const {
    Client *client = mPassRequest->GetClient();

    if (client->EnteredPassword() && client->EnteredUserInfo() && client->EnteredNickName())
        HandleError(ERR_ALREADYREGISTRED);

    if (client->EnteredPassword())
        return;

    if (mPassRequest->GetPassword() != PASSWORD)
        HandleError(CLOSING_LINK);
}

void PassValidator::SetPassRequest(PassRequest *passRequest) {
    mPassRequest = passRequest;
}
