#include "LoggingHandler.hpp"
#include <iostream>

int main(void)
{
    LOG_TRACE("trace");
    LOG_DEBUG("debug");
    LOG_INFO("info");
    LOG_WARN("warn");
    LOG_ERROR("error");
    std::cout << "log level: " << LOG_LEVEL << std::endl;

    return 0;
}