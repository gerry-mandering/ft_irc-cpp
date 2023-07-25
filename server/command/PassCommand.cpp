#include "PassCommand.hpp"

void PassCommand::Execute(Request *request) const {
    //TODO 검증 로직은 Validator에서 거치고 온다
    Client *client = request;
    request->mClient->SetPasswordEntered();
}