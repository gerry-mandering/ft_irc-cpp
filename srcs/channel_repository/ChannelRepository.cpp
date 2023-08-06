#include "ChannelRepository.hpp"

SharedPtr< Channel > ChannelRepository::CreateChannel(const std::string &name)
{
    SharedPtr< Channel > channel(new Channel(name));

    mChannels[name] = channel;

    LOG_TRACE("ChannelRepository CreateChannel()");

    return channel;
}

SharedPtr< Channel > ChannelRepository::FindByName(const std::string &name)
{
    if (mChannels.find(name) != mChannels.end())
    {
        LOG_TRACE("ChannelRepository FindByName() - Found");

        return mChannels[name];
    }

    LOG_TRACE("ChannelRepository FindByName() - NotFound");

    return SharedPtr< Channel >();
}

void ChannelRepository::RemoveChannel(const std::string &name)
{
    LOG_DEBUG("ChannelRepository RemoveChannel() - " << name);
    std::map< std::string, SharedPtr< Channel > >::iterator iter = mChannels.find(name);
    if (iter == mChannels.end())
    {
        LOG_WARN("ChannelRepository RemoveChannel() - Channel not found");
        return;
    }
    else
    {
        LOG_DEBUG("ChannelRepository RemoveChannel() - Channel removed");
    }
    mChannels.erase(iter);
}

int ChannelRepository::GetNumberOfChannels() const
{
    return mChannels.size();
}