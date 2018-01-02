/****************************************************
 * file: tcpserver.h
 * written: 11.12.2017
 * last modified:
 * synopsis: Interface for the tcp-server class
 * author: Vladyslav Podilnyk
 * **************************************************/

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <functional>
#include "threadpool.h"
#include "netpod_helpers.h"


namespace netpod {
namespace V1_0 {

using cb_t = std::function <void (bool &)>;

static constexpr size_t buffer_size {26};

struct SessionData
{
    int                                                            sock;                  // socket port
    std::string                                                    msg;                   // message from server to client if request is received
    struct sockaddr_in                                             client_addr;           // client address
    socklen_t                                                      client_sock_len;       // size of structure that stores client infomation
};


struct Config
{
    int                                                             sock_port;             // socket port
    int                                                             buf_size;              // size of ther buffer where received information are stored
    int                                                             back_log;              // queue of pending requests
    int                                                             num_of_threads;        // number of threads that working with users
    std::string                                                     acknowledge_msg;       // message from server to client if request is received
    std::string                                                     addr;                  // socket address
    cb_t                                                            cb;                    // callback wich controls work of the server
};


class Server 
{
    public:
        Server();
        Server(const Config & user_config);
        void run();
        void stop();
        Server& set_config(const Config & user_config);
        void set_cb(const cb_t & cb) {defconfig.cb = cb;};
        ~Server() = default;

    private:
        bool is_continue;
        Config defconfig;

        // function that actually handling requests from user while connection is established
        static void session(SessionData data);
        static void print_data(const std::array<char, buffer_size> & data);
};
    
} // V1_0
} // netpod

#endif // TCPSERVER_H
