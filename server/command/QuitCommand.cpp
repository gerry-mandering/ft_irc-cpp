#include "QuitCommand.hpp"

void QuitCommand::Execute() const {}

void QuitCommand::SetQuitRequest(QuitRequest *quitRequest) {
    mQuitRequest = quitRequest;
}
