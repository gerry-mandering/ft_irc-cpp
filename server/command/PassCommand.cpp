#include "PassCommand.hpp"

void PassCommand::Execute() const {
    Client *client = mPassRequest->GetClient();

    //검증 작업은 Validator 에서 진행
    client->SetPasswordEntered();
}

void PassCommand::SetPassRequest(PassRequest *passRequest) {
    mPassRequest = passRequest;
}
