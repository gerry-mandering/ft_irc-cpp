#include "ChannelRepository.hpp"

Channel *ChannelRepository::CreateChannel(const std::string &name)
{
    SharedPtr< Channel > channel(new Channel(name));

    mChannels[name] = channel;

    LOG_TRACE("ChannelRepository CreateChannel()");

    return channel.GetPtr();
}

Channel *ChannelRepository::FindByName(const std::string &name)
{
    if (mChannels.find(name) != mChannels.end())
    {
        LOG_TRACE("ChannelRepository FindByName() - Found");

        return mChannels[name].GetPtr();
    }

    LOG_TRACE("ChannelRepository FindByName() - NotFound");

    return NULL;
}

int ChannelRepository::GetNumberOfChannels() const
{
    return mChannels.size();
}