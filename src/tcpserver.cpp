/********************************************************
 * file: tcpserver.cpp
 * written: 11.12.2017
 * last modified: 11.12.2017
 * synopsis: Implementation of tcp-client/server classes
 * author: Vladyslav Podilnyk
 * *****************************************************/

#include "tcpserver.h"

namespace netpod {
namespace V1_0 {

void Server::run()
{
    ssize_t bytes_recived, bytes_send;
    int sock;
    std::string buffer;//[defconfig.buf_size];
    struct sockaddr_in sock_addr, client_sock_addr;
    int tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t client_sock_len;

    if (tcp_sock < 0)
        throw FailedToCreateSocket();

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = ntohs(defconfig.sock_port);

    if (defconfig.addr == "ANY")
        sock_addr.sin_addr.s_addr = ntohl(INADDR_ANY);
    else if (inet_aton(defconfig.addr.c_str(), &sock_addr.sin_addr))
        throw InvalidAddress();

    if (bind(tcp_sock, (struct sockaddr *) &sock_addr ,sizeof(sock_addr)) < 0)
        throw FailedToBindSocket();

    if (listen(tcp_sock, defconfig.back_log) < 0)
        throw FailedToMakeListeningSocket();

    is_running = true;

    while (is_running) {
        sock = accept(tcp_sock, (struct sockaddr *) &client_sock_addr,
                      &client_sock_len);
        if(sock < 0)
            throw FailedToCreateSocket();

        while ((bytes_recived = recv(sock, (void*)buffer.c_str(), defconfig.buf_size, 0)) > 0) {
            printf("Received msg: %li:bytes %s\n", bytes_recived,
                                                   buffer);

            bytes_send = sendto(sock, defconfig.acknowledge_msg.c_str(), 
                                sizeof(defconfig.acknowledge_msg), 0,
                                (struct sockaddr *) &client_sock_addr,
                                sizeof(client_sock_addr));

            if (bytes_send < 0)
                throw FailedToSendData();

            printf("Sent acknowledge msg: %li:bytes %s\n", bytes_send,
                                                           defconfig.acknowledge_msg);
        }

        if (bytes_recived < 0)
            throw FailedToReceiveData();

        if (defconfig.cb) {
            defconfig.cb (buffer, is_running);
        }
    }

    close(sock);
    close(tcp_sock);
}

void Server::stop()
{
    is_running = false;
}

Server& Server::set_config(const config & user_config)
{

}

} // V1_0
} // netpod
