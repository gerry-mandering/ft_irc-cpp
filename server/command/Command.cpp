#include "CapCommand.hpp"
#include "InviteCommand.hpp"
#include "JoinCommand.hpp"
#include "KickCommand.hpp"
#include "ModeCommand.hpp"
#include "NickCommand.hpp"
#include "PartCommand.hpp"
#include "PassCommand.hpp"
#include "PingCommand.hpp"
#include "PrivmsgCommand.hpp"
#include "QuitCommand.hpp"
#include "TopicCommand.hpp"
#include "UserCommand.hpp"
#include "Command.hpp"

Command* Command::mCommands[Command::NUMBER_OF_COMMANDS] = {
        new CapCommand(),
        new InviteCommand(),
        new JoinCommand(),
        new KickCommand(),
        new ModeCommand(),
        new NickCommand(),
        new PartCommand(),
        new PassCommand(),
        new PingCommand(),
        new PrivmsgCommand(),
        new QuitCommand(),
        new TopicCommand(),
        new UserCommand()
};

Command *Command::GetCommand(eRequestType requestType) {
    return mCommands[requestType];
}