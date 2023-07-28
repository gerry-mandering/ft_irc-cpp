#include "ChannelRepository.hpp"

Channel *ChannelRepository::CreateChannel(const std::string &name)
{
    Channel *channel = new Channel(name);

    mChannels[name] = channel;
    return channel;
}

Channel *ChannelRepository::FindByName(const std::string &name) const
{
    mChannels.find(name)
}