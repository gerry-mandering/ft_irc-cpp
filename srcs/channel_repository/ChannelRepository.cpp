#include "ChannelRepository.hpp"

Channel *ChannelRepository::CreateChannel(const std::string &name)
{
    Channel *channel = new Channel(name);

    mChannels[name] = channel;

    LOG_TRACE("ChannelRepository CreateChannel()");

    return channel;
}

Channel *ChannelRepository::FindByName(const std::string &name)
{
    if (mChannels.find(name) != mChannels.end())
    {
        LOG_TRACE("ChannelRepository FindByName() - Found");
        return mChannels[name];
    }

    LOG_TRACE("ChannelRepository FindByName() - NotFound");

    return NULL;
}