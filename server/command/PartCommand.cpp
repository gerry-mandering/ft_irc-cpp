#include "PartCommand.hpp"

void PartCommand::Execute() const {}

void PartCommand::SetPartRequest(PartRequest *partRequest) {
    mPartRequest = partRequest;
}
