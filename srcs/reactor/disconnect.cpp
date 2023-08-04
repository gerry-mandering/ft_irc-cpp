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

void disconnect(handle_t socket)
{
    // TODO: 추후 삭제
    std::cerr << RED << "disconnect" << COLOR_RESET << std::endl;
    Reactor *reactor = Reactor::GetInstance();
    EventHandler *handler = reactor->getHandler(socket);
    ClientRepository *clientRepository = ClientRepository::GetInstance();
    SharedPtr< Client > client = clientRepository->FindBySocket(socket);

    reactor->unregisterHandler(handler);
    close(socket);
    if (!client.GetPtr())
    {
        LOG_INFO("Client not found for socket " << socket);
        return;
    }
    clientRepository->RemoveClient(client->GetSocket(), client->GetNickName());
    // TODO: shared ptr로 변환 (클라이언트 삭제할 때 채널 없을 수도..?)
    Channel *channel = client->GetChannel();
    if (!channel)
    {
        LOG_INFO("Client is not in any channel");
        return;
    }
    if (channel->GetClientCount() == 1)
    {
        LOG_INFO("Client is the only one in channel " << channel->GetName());
        ChannelRepository *channelRepository = ChannelRepository::GetInstance();
        channelRepository->RemoveChannel(channel->GetName());
        return;
    }
    channel->RemoveClient(client->GetNickName());
}
