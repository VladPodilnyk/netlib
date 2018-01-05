#include "netlib/tcpclient.h"

using namespace netpod::V1_0;

int main()
{
    Client client;
    client.request("Hello, Do you wanna know?");
    return EXIT_SUCCESS;
}