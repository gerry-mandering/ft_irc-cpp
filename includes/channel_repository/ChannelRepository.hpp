#pragma once

#include "Channel.hpp"
#include "LoggingHandler.hpp"
#include "SingletonTemplate.hpp"
#include <iostream>
#include <map>

class ChannelRepository : public TSingleton< ChannelRepository >
{

  public:
    // JOIN은 있으면 만들지만 나머지는 예외처리 ! 채널 만드는 거 따로
    Channel *CreateChannel(const std::string &name);
    Channel *FindByName(const std::string &name);

    int GetNumberOfChannels() const;

  private:
    // TODO shared_ptr
    std::map< std::string, Channel * > mChannels;
};
