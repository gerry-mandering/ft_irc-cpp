#include "EventHandler.hpp"

EventHandler::EventHandler() : m_handle(0) {}

EventHandler::EventHandler(handle_t handle) : m_handle(handle) {}

EventHandler::~EventHandler() {}

handle_t EventHandler::getHandle() const
{
    return m_handle;
}