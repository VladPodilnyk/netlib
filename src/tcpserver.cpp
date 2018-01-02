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
    defconfig.back_log = 5;
    defconfig.num_of_threads = 8;
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
    defconfig.back_log = user_config.back_log;
    defconfig.num_of_threads = user_config.num_of_threads;
    defconfig.acknowledge_msg = user_config.acknowledge_msg;
    defconfig.addr = user_config.addr;
    defconfig.cb = user_config.cb;
    is_continue = false;
}


void Server::session(SessionData data)
{
    std::array<char, buffer_size> buffer;
    ssize_t bytes_received, bytes_send;

    while ((bytes_received = recv(data.sock, std::begin(buffer), buffer_size - 1, 0)) > 0) {
        buffer[buffer_size - 1] = '\0';
        std::cout << "Data received: ";
        print_data(buffer);

        bytes_send = sendto(data.sock, data.msg.c_str(), 
                            data.msg.size(), 0,
                            (struct sockaddr *) &data.client_addr,
                            data.client_sock_len);

        if (bytes_send < 0)
            throw FailedToSendData();
    }

    shutdown(data.sock, SHUT_RDWR);
    close(data.sock);
}


void Server::run()
{
    int sock;
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

    ThreadPool *thread_pool = new ThreadPool(defconfig.num_of_threads);

    do {

        sock = accept(tcp_sock, (struct sockaddr *) &client_sock_addr,
                      &client_sock_len);

        if(sock < 0)
            throw FailedToCreateSocket();
        
        SessionData data { 
                            sock,
                            defconfig.acknowledge_msg,
                            client_sock_addr,
                            client_sock_len
                         };

        try {
            thread_pool->push(std::bind(&Server::session, data));

        } catch (QueueOverflow e) {
            defconfig.cb = nullptr;
            is_continue = false;
        }

        if (defconfig.cb) {
            defconfig.cb (is_continue);
        }

    } while (is_continue);

    delete thread_pool;
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


void Server::print_data(const std::array<char, buffer_size> & data)
{
    for (size_t i = 0; i < data.size(); ++i)
        std::cout << data[i];

    std::cout << std::endl;   
}

} // V1_0
} // netpod
