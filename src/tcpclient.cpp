/********************************************************
 * file: tcpclient.cpp
 * written: 11.12.2017
 * last modified:
 * synopsis: Implementation of tcp-client class
 * author: Vladyslav Podilnyk
 * *****************************************************/

#include "netlib/tcpclient.h"

namespace netpod {
namespace V1_0 {

Client::Client()
{
    defsetting.sock_port = 3001;   // default settings for the client
    defsetting.buf_size = 10;
    defsetting.addr = "LOOPBACK"; 
}


Client::Client(const ClientSettings & usr_settings)
{
    defsetting.sock_port = usr_settings.sock_port;
    defsetting.buf_size = usr_settings.buf_size;
    defsetting.addr = usr_settings.addr;
}


int Client::request(const std::string & data)
{
    ssize_t bytes_send, bytes_received;
    struct sockaddr_in sock_addr, serv_sock_addr;
    int tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    std::string buffer;

    if (tcp_sock < 0)
        throw FailedToCreateSocket();

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(defsetting.sock_port);

    if (defsetting.addr == "LOOPBACK")
        sock_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    else if (inet_aton(defsetting.addr.c_str(), &sock_addr.sin_addr) == 0)
        throw InvalidAddress();

    if (connect(tcp_sock, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0)
        throw FailedToEstablishConnection();

    bytes_send = sendto(tcp_sock, data.c_str(), data.size(), 0,
                        (struct sockaddr *) &serv_sock_addr,
                        sizeof(serv_sock_addr));

    shutdown(tcp_sock, SHUT_WR);
    if (bytes_send < 0)
        throw FailedToSendData();
    printf("Data sent: %li:bytes %s\n", bytes_send, data.c_str());

    bytes_received = recv(tcp_sock, (void*)buffer.c_str(), defsetting.buf_size, 0);

    if (bytes_received < 0)
            throw FailedToReceiveData();
    printf("Data received: %li:bytes %s\n", bytes_received, buffer.c_str());

    shutdown(tcp_sock, SHUT_RDWR);
    close(tcp_sock);
    return bytes_received;
}

} // V1_0
} // netpod