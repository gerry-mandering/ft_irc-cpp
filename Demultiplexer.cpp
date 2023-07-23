#include "Demultiplexer.hpp"

#include <iostream>

Demultiplexer::Demultiplexer() : m_numHandlers(0) {}

Demultiplexer::~Demultiplexer() {}

void Demultiplexer::setNumHandlers(size_t numHandlers)
{
    m_numHandlers = numHandlers;
}