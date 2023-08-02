#include "StreamHandler.hpp"
#include "ClientRepository.hpp"
#include "LoggingHandler.hpp"
#include "def.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <vector>

const std::string StreamHandler::CRLF = "\r\n";

StreamHandler::StreamHandler(handle_t handle) : EventHandler(OFF_EVENT), m_handle(handle) {}

StreamHandler::~StreamHandler() {}

handle_t StreamHandler::getHandle(void) const
{
    return (m_handle);
}

int StreamHandler::handleRead(void)
{
    char tmpBuf[TMP_BUF_SIZE];
    ssize_t nread;
    size_t crlf_pos;
    std::string requestStr;

    std::memset(tmpBuf, 0, sizeof(tmpBuf));
    nread = read(m_handle, tmpBuf, sizeof(tmpBuf) - 1);
    LOG_TRACE("nread: " << nread);
    if (nread < 0)
    {
        LOG_DEBUG("StreamHandler read failed: " << strerror(errno));
        return (OK);
    }
    if (nread == 0)
    {
        LOG_DEBUG("StreamHandler read eof: " << strerror(errno));
        // TODO: disconnect(구현 자유롭게 하면 될듯 되도록 disconnect 하도록)
        return (OK);
    }
    std::string tcpStreams(tmpBuf);
    LOG_TRACE("[ " << m_handle << " ]"
                   << " sent data: " << tcpStreams);
    m_readBuf += tcpStreams;
    while (hasRequest(requestStr) != false)
    {
        Request *request;

        try
        {
            LOG_DEBUG("Try parse: " << requestStr);
            request = Parser::parseRequest(requestStr, m_handle);
            LOG_DEBUG("Parsing success");
        }
        catch (std::exception &e)
        {
            // TODO: 에러 핸들러 호출로 처리
            LOG_DEBUG("Failed to parese request: " << requestStr << e.what());
            continue;
        }
        // TODO: registerEvent(handler, WRITE_EVENT)는 다른 곳에서 필요할 때 등록 (response 큐에 집어넣고 호출하면 될듯)
        // handler는 reactor가 싱글톤이니 map에서 fd로 찾으면 됨
        //    return (g_reactor().registerEvent(this, WRITE_EVENT));

        //     TODO: 민석님이 작성하실 부분
        Validator *validator = Validator::GetInstance();
        if (request->Accept(validator))
        {
            Executor *executor = Executor::GetInstance();
            request->Accept(executor);
        }
    }

    // Log_TRACE("socket fd: " << m_handle << " is closed\n");
    //    return (g_reactor().registerEvent(this, WRITE_EVENT));

    // std::cerr << "before validating\n";
    // Validator *validator = Validator::GetInstance();
    // if (request->Accept(validator))
    // {
    //     std::cerr << "after validating\n";
    //     Executor *executor = Executor::GetInstance();
    //     request->Accept(executor);
    // }
    // std::cerr << "after execution\n";

    // Log_TRACE("socket fd: " << m_handle << " is closed\n");
    // return true;
    return (0);
}

int StreamHandler::handleWrite(void)
{
    // 레거시 추후 삭제 예정
    // Client *client = ClientRepository::GetInstance()->FindBySocket(m_handle);
    // std::string responseStr;
    // ssize_t nwrite;

    // if (!client)
    // {
    //     LOG_ERROR("StreamHandler write event but no client found" << strerror(errno));
    //     exit(EXIT_FAILURE);
    // }
    // // 일단 한번에 다 더해서 처리, irssi 내부구현 따라 될수도 안될수도
    // while (client->HasResponse())
    //     responseStr += client->ExtractResponse();
    // nwrite = write(m_handle, responseStr.c_str(), responseStr.size());
    // if (nwrite < 0)
    // {
    //     LOG_DEBUG("StreamHandler write failed: " << strerror(errno));
    //     return (OK);
    // }
    // if (nwrite < responseStr.size())
    // {
    //     LOG_ERROR("Partial write, NOT IMPLEMENTED YET");
    //     exit(EXIT_FAILURE);
    // }
    // return (g_reactor().unregisterEvent(this, WRITE_EVENT));

    Client *client = ClientRepository::GetInstance()->FindBySocket(m_handle);
    ssize_t nwrite;

    // TODO: 일단 한번에 버퍼에 있는 내용 다 클라이언트에 보내봄. 아마 에러 없을 것임
    if (!client)
    {
        LOG_ERROR("StreamHandler write event but no client found" << strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (m_writeBuf.empty())
    {
        LOG_ERROR("StreamHandler write event but buf is empty");
        exit(EXIT_FAILURE);
    }
    nwrite = write(m_handle, m_writeBuf.c_str(), m_writeBuf.size());
    if (nwrite < 0)
    {
        // TODO: strerror cstring 사용
        LOG_DEBUG("StreamHandler write failed write again or disconnect: " << strerror(errno));
        return (OK);
    }
    if (nwrite < m_writeBuf.size())
    {
        LOG_ERROR("Partial write");
        m_writeBuf = m_writeBuf.substr(nwrite);
        return (OK);
    }
    if (nwrite == m_writeBuf.size())
    {
        LOG_DEBUG("StreamHandler write success fully: " << m_writeBuf);
        m_writeBuf.clear();
        return (g_reactor().unregisterEvent(this, WRITE_EVENT));
    }
    LOG_ERROR("StreamHandler write: should not reach here " << strerror(errno));
    return (OK);

    // 기존 에코서버 로직
    // if (write(m_handle, m_readBuf.c_str(), m_readBuf.size()) < 0)
    //     LOG_DEBUG("StreamHandler write failed: " << strerror(errno));
    // m_readBuf.clear();
    // return (g_reactor().unregisterEvent(this, WRITE_EVENT));
}

int StreamHandler::handleError(void)
{
    return (0);
}

bool StreamHandler::hasRequest(std::string &requestStr)
{
    size_t crlf_pos = m_readBuf.find(CRLF);
    if (crlf_pos == std::string::npos)
        return (false);
    LOG_TRACE("crlf_pos: " << crlf_pos)
    while (crlf_pos > MAX_MSG_SIZE - CRLF_LEN)
    {
        m_readBuf = m_readBuf.substr(MAX_MSG_SIZE - CRLF_LEN);
        crlf_pos = m_readBuf.find(CRLF);
        LOG_TRACE("crlf_pos: " << crlf_pos)
    }
    requestStr = m_readBuf.substr(0, crlf_pos + CRLF_LEN);
    LOG_DEBUG("Extracted request: " << requestStr);
    m_readBuf = m_readBuf.substr(crlf_pos + CRLF_LEN);
    LOG_DEBUG("Reamin buffer len: " << m_readBuf.size() << ", " << m_readBuf);
    return (true);
}

void StreamHandler::addResponseToBuf(const std::string &responseStr)
{
    LOG_DEBUG("Add response to handler's wrbuf: " << responseStr);
    g_reactor().registerEvent(this, WRITE_EVENT);
    m_writeBuf += responseStr;
}
