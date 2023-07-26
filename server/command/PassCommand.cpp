#include "PassCommand.hpp"

void PassCommand::Execute() const {}

void PassCommand::setPassRequest(PassRequest *passRequest) {
    mPassRequest = passRequest;
}
