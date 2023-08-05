#include "disconnect.h"
#include "Channel.hpp"
#include "ChannelRepository.hpp"
#include "Client.hpp"
#include "ClientRepository.hpp"
#include "EventHandler.hpp"
#include "LoggingHandler.hpp"
#include "Reactor.hpp"
#include "SharedPtr.hpp"
#include "color.h"
#include <unistd.h>

void disconnectClient(handle_t socket)
{
    LOG_INFO(RED << "[ " << socket << " ]: "
                 << "disconnectClient" << COLOR_RESET);
    ClientRepository *clientRepository = ClientRepository::GetInstance();
    SharedPtr< Client > client = clientRepository->FindBySocket(socket);

    if (!client)
    {
        LOG_INFO(" Client not found for socket " << socket);
        return;
    }
    clientRepository->RemoveClient(client->GetSocket(), client->GetNickName());
    SharedPtr< Channel > channel = client->GetChannel();
    if (!channel)
    {
        LOG_INFO(" Client is not in any channel");
        return;
    }
    if (channel->GetClientCount() == 1)
    {
        LOG_INFO(" Client leave and remove channel " << channel->GetName());
        ChannelRepository *channelRepository = ChannelRepository::GetInstance();
        channel->RemoveClient(client->GetNickName());
        channelRepository->RemoveChannel(channel->GetName());
        return;
    }
    LOG_INFO(" Client leave and not remove channel " << channel->GetName());
    channel->RemoveClient(client->GetNickName());
}
