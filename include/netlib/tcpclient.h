/****************************************************
 * file: tcpclient.h
 * written: 11.12.2017
 * last modified:
 * synopsis: Interface for the tcp-client class
 * author: Vladyslav Podilnyk
 * **************************************************/

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

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
#include "netpod_helpers.h"


namespace netpod {
namespace V1_0 {

struct ClientSettings
{
    int sock_port;         // socket port
    int buf_size;          // size of ther buffer where received information are stored
    std::string addr;      // socket address, "LOOPBACK" by default
};

class Client
{
    public:
        Client();
        Client(const ClientSettings & usr_settings);
        int request(const std::string & data);
        ~Client() = default;

    private:
        ClientSettings defsetting;
};

} // V1_0
} // netpod

#endif // TCPCLIENT_H