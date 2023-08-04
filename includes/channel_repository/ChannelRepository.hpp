#pragma once

#include "Channel.hpp"
#include "LoggingHandler.hpp"
#include "SharedPtr.hpp"
#include "SingletonTemplate.hpp"
#include <iostream>
#include <map>

class ChannelRepository : public TSingleton< ChannelRepository >
{

  public:
    Channel *CreateChannel(const std::string &name);
    Channel *FindByName(const std::string &name);

    int GetNumberOfChannels() const;

  private:
    std::map< std::string, SharedPtr< Channel > > mChannels;
};
