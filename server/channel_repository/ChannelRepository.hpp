#ifndef CHANNEL_REPOSITORY_H
#define CHANNEL_REPOSITORY_H

#include <iostream>
#include <map>
#include "../channel/Channel.hpp"

class ChannelRepository {

public:
    ChannelRepository();
    ~ChannelRepository();

    //JOIN은 있으면 만들지만 나머지는 예외처리 ! 채널 만드는 거 따로
    Channel *FindByName(std::string &channerName) const;

private:
    std::map<std::string, Channel *> mChannels;

};

#endif