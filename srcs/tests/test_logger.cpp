#include "LoggingHandler.hpp"
#include <iostream>

int main(void)
{
    int i = 0;

    LOG_TRACE("trace log " << i);
    LOG_DEBUG("debug log " << i);
    LOG_INFO("info log ");
    LOG_WARN("warn log ");
    LOG_ERROR("error log ");
    // LoggingHandler tmp;
    // LoggingHandler &logger = *(LoggingHandler::GetInstance());

    // logger << LoggingHandler::buildPrefix(LEVEL_TRACE);
    // logger << "trace\n";
    // std::cout << LoggingHandler::GetInstance()->getWriteBuf();

    return 0;
}