#include "ModeCommand.hpp"

void ModeCommand::Execute() const {}

void ModeCommand::setModeRequest(ModeRequest *modeRequest) {
    mModeRequest = modeRequest;
}
