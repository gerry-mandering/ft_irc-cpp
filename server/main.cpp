#include "connection_initializer/ConnectionInitializer.hpp"
#include "reactor/Reactor.hpp"

int main(void) {

    ConnectionInitializer *connectionInitializer = ConnectionInitializer::GetInstance();
    Reactor *reactor = Reactor::GetInstance();

    reactor->RegisterHandler(connectionInitializer, reactor::EVENT_READ);

    while (true)
        reactor->HandleEvents();

    return 0;

}
