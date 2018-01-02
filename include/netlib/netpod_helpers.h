/****************************************************
 * file: netpod_helpers.h
 * written: 11.12.2017
 * last modified: 12.12.2017
 * synopsis: Definition of exceptions for netlib
 * author: Vladyslav Podilnyk
 * **************************************************/

#ifndef NETPOD_HELPERS_H
#define NETPOD_HELPERS_H

#include <exception>
#include <string>

namespace netpod {

class NetBaseException : public std::exception
{
    public:
        NetBaseException() : error_msg{} {};
        NetBaseException(const std::string & msg) : error_msg{msg} {};
        const char* what() const throw() override
        {
            return error_msg.c_str();
        }

    protected:
        std::string error_msg;
};


class FailedToCreateSocket : public NetBaseException
{
    public:
        FailedToCreateSocket() : NetBaseException("Failed to create socket.") {};
};


class FailedToBindSocket : public NetBaseException
{
    public:
        FailedToBindSocket() : NetBaseException("Failed to give socket an address.") {};
};


class FailedToMakeListeningSocket : public NetBaseException
{
    public:
        FailedToMakeListeningSocket() : NetBaseException("Failed to make listening socket.") {};
};


class FailedToSendData : public NetBaseException
{
    public:
        FailedToSendData() : NetBaseException("Failed to send data to a client.") {};
};


class FailedToReceiveData : public NetBaseException
{
    public:
        FailedToReceiveData() : NetBaseException("Failed to receive data from a client.") {};
};


class FailedToEstablishConnection : public NetBaseException
{
    public:
        FailedToEstablishConnection() : NetBaseException("Failed to establish connection.") {};
};


class InvalidAddress : public NetBaseException
{
    public:
        InvalidAddress() : NetBaseException("Bad host address is given.") {};
};


class QueueOverflow : public NetBaseException
{
    public:
        QueueOverflow() : NetBaseException("Number of tasks is out of range.") {};
};

} // netpod

#endif // NETPOD_HELPERS_H