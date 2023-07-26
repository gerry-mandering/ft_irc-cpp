#include "PrivmsgCommand.hpp"

void PrivmsgCommand::Execute() const {}

void PrivmsgCommand::setPrivmsgRequest(PrivmsgRequest *privmsgRequest) {
    mPrivmsgRequest = privmsgRequest;
}
