#include "AcceptHandler.hpp"
#include "KqueueDemultiplexer.hpp"
#include "Reactor.hpp"
#include "def.h"
#include <iostream>

#define PORT 6667
#define PASSWORD "1234"

int main(int argc, char **argv)
{
    (void)argv;
    // TODO EnvManager 객체 만들기
    if (argc != 3)
    {
        std::cerr << "Wrong Parameter." << std::endl;
        return (0);
    }
    // EnvManager::GetInstance()->SetEnv(argc, argv);

    AcceptHandler acceptHandler(PORT, PASSWORD);
    g_reactor().setDemultiplexer(new KqueueDemultiplexer());

    if (!acceptHandler.init())
    {
        std::cerr << "Failed to initialize accept handler." << std::endl;
        return (1);
    }
    while (true)
        g_reactor().handleEvents();
    return (0);
}