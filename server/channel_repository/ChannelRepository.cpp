#include "ChannelRepository.hpp"

Channel *ChannelRepository::CreateChannel(std::string &name) {
    Channel *channel = new Channel(name);

    mChannels[name] = channel;
    return channel;
}