#include "StreamHandler.hpp"
#include "LoggingHandler.hpp"
#include "def.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <vector>

const std::string StreamHandler::CRLF = "\r\n";

StreamHandler::StreamHandler(handle_t handle) : m_handle(handle) {}

StreamHandler::~StreamHandler() {}

handle_t StreamHandler::getHandle(void) const
{
    return (m_handle);
}

bool StreamHandler::hasRequest(std::string &requestStr)
{
    size_t crlf_pos = m_buf.find(CRLF);
    if (crlf_pos == std::string::npos)
        return (false);
    LOG_TRACE("crlf_pos: " << crlf_pos)
    while (crlf_pos > MAX_MSG_SIZE - CRLF_LEN)
    {
        m_buf = m_buf.substr(MAX_MSG_SIZE - CRLF_LEN);
        crlf_pos = m_buf.find(CRLF);
        LOG_TRACE("crlf_pos: " << crlf_pos)
    }
    requestStr = m_buf.substr(0, crlf_pos + CRLF_LEN);
    LOG_DEBUG("Extracted request: " << requestStr);
    m_buf = m_buf.substr(crlf_pos + CRLF_LEN);
    LOG_DEBUG("Reamin buffer len: " << m_buf.size() << ", " << m_buf);
    return (true);
}

int StreamHandler::handleRead(void)
{
    char tmpBuf[4096];
    ssize_t nread;
    size_t crlf_pos;
    std::string requestStr;

    std::memset(tmpBuf, 0, sizeof(tmpBuf));
    // TODO: nc에서 eof만 보내면 연결 끊김
    nread = read(m_handle, tmpBuf, sizeof(tmpBuf));
    if (nread <= 0)
    {
        LOG_DEBUG("StreamHandler read failed: " << strerror(errno));
        // TODO: 리턴 코드 (어차피 0이긴 할텐데..)
        return (OK);
    }
    std::string tcpStreams(tmpBuf);
    LOG_TRACE("[ " << m_handle << " ]"
                   << " sent data: " << tcpStreams);
    m_buf += tcpStreams;
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

// TODO: 시간 나면 partial write까지 처리
// TODO: 클라이언트를 fd로 검색해서 가져와야 response 큐에서 갖고와야 함.
int StreamHandler::handleWrite(void)
{
    // handleWrite 로직 틀
    // ResponsePtr response;

    // response = m_responseQueue.front();
    // std::string responseStr = response->toString();
    // ssize_t nwrite;

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
    // m_responseQueue.pop();
    // if (m_responseQueue.empty())
    //     return (g_reactor().unregisterEvent(this, WRITE_EVENT));
    // return (OK);

    // 기존 에코서버 로직
    if (write(m_handle, m_buf.c_str(), m_buf.size()) < 0)
        LOG_DEBUG("StreamHandler write failed: " << strerror(errno));
    m_buf.clear();
    return (g_reactor().unregisterEvent(this, WRITE_EVENT));
}

int StreamHandler::handleError(void)
{
    return (0);
}