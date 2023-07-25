#include "RequestHandler.hpp"

RequestHandler::RequestHandler() {}

RequestHandler::~RequestHandler() {}

RequestHandler::RequestHandler(const reactor::Socket clientSocket) : mClientSocket(clientSocket) {}

reactor::Socket RequestHandler::GetSocket() const {
    return mClientSocket;
}

void RequestHandler::HandleRead() {
    char buffer[1024];

    std::memset(buffer, 0, sizeof(buffer));

    //Kernel buffer size 수정?
    int length = recv(mClientSocket, buffer, 1024, 0);
    if (length == -1) {
        std::cerr << "Fail to receive client request." << std::endl;
    }


    PacketManager()->handlePacket(std::string recv);
//    1. 정상적으로 읽혀서 끝에 \r\n 있는경우 - std::string buffer 담고 끝에 \r\n 있으면 바로 파싱
//    2. partial read 커널버퍼에 더 남아있는 경우 - std::string buffer의 max 사이즈 만큼 들어왔는데 \r\n 이 없는경우 버퍼에
//    담고 다시 read;
//    3. 두번 request 중첩해서 오는 경우 - find 했는데 \r\n 뒤에 문자열이 남아있는경우 잘라내서 앞부분은 파싱 뒤는 버퍼에 저장 / 213\r\nmsg    sdsd\r\n
//    4. 213\r\n msg\r\n aafad
//    5. 비정상적인 \r\n 없는 경우
//
//    PaketManager(fd)
//    -> clientRepo 조회 파서 보내고 남은거는 readbuf 저장

    std::cout << buffer << std::endl;

}

void RequestHandler::HandleWrite() {

}

void RequestHandler::HandleError() {

}

