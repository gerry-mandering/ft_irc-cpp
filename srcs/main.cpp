#include "AcceptHandler.hpp"
#include "EnvManager.hpp"
#include "KqueueDemultiplexer.hpp"
#include "Reactor.hpp"
#include "def.h"
#include "parser.h"
#include <iostream>
#include <string>

#define PORT 6667
#define PASSWORD "1234"

int main(int argc, char **argv)
{
    (void)argv;
    if (argc != 3)
    {
        std::cerr << "Wrong Parameter." << std::endl;
        return (0);
    }
    EnvManager::GetInstance()->SetPortNumber(std::string(argv[1]));
    EnvManager::GetInstance()->SetConnectionPassword(std::string(argv[2]));

    Parser::initParsers();

    // TODO AcceptHandler 내부에서 불러오는 방식으로 수정?
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