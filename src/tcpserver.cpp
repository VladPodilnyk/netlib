/********************************************************
 * file: tcpserver.cpp
 * written: 11.12.2017
 * last modified:
 * synopsis: Implementation of tcp-server class
 * author: Vladyslav Podilnyk
 * *****************************************************/

#include "netlib/tcpserver.h"

namespace netpod {
namespace V1_0 {

Server::Server()
{
    defconfig.sock_port = 3001;          // default configuration
    defconfig.buf_size = 50;
    defconfig.acknowledge_msg = "Hello";
    defconfig.addr = "ANY";              // set "ANY" if you don't care about address
                                         //   which will be binded to the socket
    defconfig.cb = nullptr;              // please provide callback in order to control server
    is_continue = false;
}


Server::Server(const Config & user_config)
{
    defconfig.sock_port = user_config.sock_port;
    defconfig.buf_size = user_config.buf_size;
    defconfig.acknowledge_msg = user_config.acknowledge_msg;
    defconfig.addr = user_config.addr;
    defconfig.cb = user_config.cb;
    is_continue = false;
}


void Server::run()
{
    ssize_t bytes_recived, bytes_send;
    int sock;
    std::string buffer;
    struct sockaddr_in sock_addr, client_sock_addr;
    int tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t client_sock_len;

    if (tcp_sock < 0)
        throw FailedToCreateSocket();

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = ntohs(defconfig.sock_port);

    if (defconfig.addr == "ANY")
        sock_addr.sin_addr.s_addr = ntohl(INADDR_ANY);
    else if (inet_aton(defconfig.addr.c_str(), &sock_addr.sin_addr) == 0)
        throw InvalidAddress();

    if (bind(tcp_sock, (struct sockaddr *) &sock_addr ,sizeof(sock_addr)) < 0)
        throw FailedToBindSocket();

    if (listen(tcp_sock, defconfig.back_log) < 0)
        throw FailedToMakeListeningSocket();

    do {
        sock = accept(tcp_sock, (struct sockaddr *) &client_sock_addr,
                      &client_sock_len);
        if(sock < 0)
            throw FailedToCreateSocket();

        while ((bytes_recived = recv(sock, (void*)buffer.c_str(), defconfig.buf_size, 0)) > 0) {
            printf("Received msg: %li:bytes %s\n", bytes_recived,
                                                   buffer.c_str());

            bytes_send = sendto(sock, defconfig.acknowledge_msg.c_str(), 
                                sizeof(defconfig.acknowledge_msg), 0,
                                (struct sockaddr *) &client_sock_addr,
                                sizeof(client_sock_addr));

            if (bytes_send < 0)
                throw FailedToSendData();

            printf("Sent acknowledge msg: %li:bytes %s\n", bytes_send,
                                                           defconfig.acknowledge_msg.c_str());
        }

        if (defconfig.cb) {
            defconfig.cb (buffer, is_continue);
        }

    } while (is_continue);

    close(sock);
    close(tcp_sock);
}

void Server::stop()
{
    is_continue = false;
}

Server& Server::set_config(const Config & user_config)
{
    defconfig.sock_port = user_config.sock_port;
    defconfig.buf_size = user_config.buf_size;
    defconfig.acknowledge_msg = user_config.acknowledge_msg;
    defconfig.addr = user_config.addr;
    defconfig.cb = user_config.cb;
    return *this;
}

} // V1_0
} // netpod
