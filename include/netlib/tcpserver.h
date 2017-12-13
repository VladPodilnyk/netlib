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
#include <string>
#include <functional>
#include "netlib/netpod_helpers.h"


namespace netpod {
namespace V1_0 {

struct Config
{
    int sock_port;                                       // socket port
    int buf_size;                                        // size of ther buffer where received information are stored
    int back_log;                                        // queue of pending requests
    std::string acknowledge_msg;                         // message from server to client if request is received
    std::string addr;                                    // socket address
    std::function <void (std::string &, bool & )> cb;    // callback wich controls work of the server
};

class Server 
{
    public:
        Server();
        Server(const Config & user_config);
        void run();
        void stop();
        Server& set_config(const Config & user_config);
        ~Server() = default;

    private:
        bool is_continue;
        Config defconfig;
};
    
} // V1_0
} // netpod

#endif // TCPSERVER_H
