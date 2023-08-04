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

int ChannelRepository::GetNumberOfChannels() const
{
    return mChannels.size();
}