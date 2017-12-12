/****************************************************
 * file: tcpserver.h
 * written: 11.12.2017
 * last modified: 11.12.2017
 * synopsis: Interface for the tcp-client/server
 * author: Vladyslav Podilnyk
 * **************************************************/

#ifndef NETPOD_H
#define NETPOD_H

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
#include "neterrors/netpod_helpers.h"


namespace netpod {
namespace V1_0 {

struct config
{
    int sock_port;                                       // socket port
    int buf_size;                                        // size of ther buffer where received information are stored
    int back_log;                                        // queue of pending requests
    std::string acknowledge_msg;                         // message from server to client if request is received
    std::string addr;                                    // socket address
    std::function <void (std::string &, bool & )> cb;    // std::function <void (const char *, bool * const)>
};

class Server 
{
    public:
        Server();
        Server(const config & user_config);
        void run();
        void stop();
        Server& set_config(const config & user_config);
        ~Server();

    private:
        bool is_running;
        config defconfig;
};
    
} // V1_0
} // netpod

#endif // NETPOD_H