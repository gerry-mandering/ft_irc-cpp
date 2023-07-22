#include "Server.hpp"
#include "Reactor.hpp"

int main(void) {

    Server *server = Server::GetInstance();
    Reactor *reactor = Reactor::GetInstance();

    reactor->RegisterHandler(server, reactor::EVENT_READ);

    while (true)
        reactor->HandleEvents();

    return 0;

}
