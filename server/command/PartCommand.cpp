#include "PartCommand.hpp"

void PartCommand::Execute() const {}

void PartCommand::setPartRequest(PartRequest *partRequest) {
    mPartRequest = partRequest;
}
