#include "LoggingHandler.hpp"
#include <iostream>

int main(void)
{
    int i = 0;

    LOG_TRACE("trace" << i);
    LOG_DEBUG("debug" << i);
    LOG_INFO("info");
    LOG_WARN("warn");
    LOG_ERROR("error");
    // LoggingHandler tmp;
    // LoggingHandler &logger = *(LoggingHandler::GetInstance());

    // logger << LoggingHandler::buildPrefix(LEVEL_TRACE);
    // logger << "trace\n";
    std::cout << LoggingHandler::GetInstance()->m_writeBuf << std::endl;

    return 0;
}