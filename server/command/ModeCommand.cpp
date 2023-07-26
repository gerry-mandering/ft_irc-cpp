#include "ModeCommand.hpp"

void ModeCommand::Execute() const {}

void ModeCommand::SetModeRequest(ModeRequest *modeRequest) {
    mModeRequest = modeRequest;
}
