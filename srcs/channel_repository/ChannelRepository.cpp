#include "ChannelRepository.hpp"

// TODO: dahkang 만들기 전에 중복검사, shared_ptr로 생성 (현재 로직은 덮어씀)
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