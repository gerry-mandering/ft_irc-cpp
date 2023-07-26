#include "PassCommand.hpp"

void PassCommand::Execute(PassRequest *passRequest) const {
    Client *client = passRequest->GetClient();

    //검증 작업은 Validator 에서 진행
    client->SetPasswordEntered();
}
