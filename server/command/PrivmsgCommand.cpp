#include "PrivmsgCommand.hpp"

void PrivmsgCommand::Execute() const {}

void PrivmsgCommand::SetPrivmsgRequest(PrivmsgRequest *privmsgRequest) {
    mPrivmsgRequest = privmsgRequest;
}
