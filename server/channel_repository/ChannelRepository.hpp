#ifndef CHANNEL_REPOSITORY_H
#define CHANNEL_REPOSITORY_H

#include <iostream>
#include <map>
#include "../channel/Channel.hpp"

class ChannelRepository {

public:
    ChannelRepository();
    ~ChannelRepository();

    Channel *FindByName() const;

private:
    std::map<std::string, Channel *> mChannels;

};

#endif