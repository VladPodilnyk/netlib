#include "netlib/tcpserver.h"

using namespace netpod::V1_0;

int main()
{
    Server server;
    size_t times = 0;
    const size_t timesMax = 1000;
    server.set_cb([&times](bool & is_continue){is_continue = times++ < timesMax;});
    server.run();
    return EXIT_SUCCESS;
}