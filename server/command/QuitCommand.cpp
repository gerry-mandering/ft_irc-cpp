#include "QuitCommand.hpp"

void QuitCommand::Execute() const {}

void QuitCommand::setQuitRequest(QuitRequest *quitRequest) {
    mQuitRequest = quitRequest;
}
