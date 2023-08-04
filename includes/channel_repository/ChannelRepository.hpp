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
    SharedPtr< Channel > CreateChannel(const std::string &name);
    SharedPtr< Channel > FindByName(const std::string &name);

    int GetNumberOfChannels() const;

  private:
    std::map< std::string, SharedPtr< Channel > > mChannels;
};
