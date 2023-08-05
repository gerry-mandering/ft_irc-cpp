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
    LOG_INFO(RED << "[ " << socket << " ]: "
                 << "disconnect" << COLOR_RESET);
    Reactor *reactor = Reactor::GetInstance();
    EventHandler *handler = reactor->getHandler(socket);
    ClientRepository *clientRepository = ClientRepository::GetInstance();
    SharedPtr< Client > client = clientRepository->FindBySocket(socket);

    reactor->unregisterHandler(handler);
    close(socket);
    if (!client)
    {
        LOG_INFO(__func__ << " Client not found for socket " << socket);
        return;
    }
    clientRepository->RemoveClient(client->GetSocket(), client->GetNickName());
    // TODO: shared ptr로 변환 (클라이언트 삭제할 때 채널 없을 수도..?)
    SharedPtr< Channel > channel = client->GetChannel();
    if (!channel)
    {
        LOG_INFO(__func__ << " Client is not in any channel");
        return;
    }
    if (channel->GetClientCount() == 1)
    {
        LOG_INFO(__func__ << " Client leave and remove channel " << channel->GetName());
        ChannelRepository *channelRepository = ChannelRepository::GetInstance();
        channelRepository->RemoveChannel(channel->GetName());
        return;
    }
    LOG_INFO(__func__ << " Client leave and not remove channel " << channel->GetName());
    channel->RemoveClient(client->GetNickName());
}
