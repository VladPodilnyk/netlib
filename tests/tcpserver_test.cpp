#include "netlib/tcpserver.h"
#include "netlib/tcpclient.h"
#include <gtest/gtest.h>


class ServerTest : public ::testing::Test
{
    public:
        void SetUp () override {};
        void TearDown () override {};
};


class ClientTest : public ::testing::Test
{
    public:
        void SetUp () override {};
        void TearDown () override {};
};


TEST_F (ServerTest, ServerWrongAdressThrow)
{
    // Arrange.
    netpod::V1_0::Config config;
    config.addr = "abra-cadabra";
    netpod::V1_0::Server s(config);

    // Act. Assert.
    EXPECT_THROW (s.run(), netpod::InvalidAddress);
}


TEST_F (ServerTest, ServerFailedToBindSocket)
{
    // Arrange 
    netpod::V1_0::Config config;
    config.addr = "ANY";
    netpod::V1_0::Server s(config);

    // Act. Assert.
    EXPECT_THROW (s.run(), netpod::FailedToBindSocket);
}


TEST_F (ServerTest, CheckCreateServer)
{
    // Arrange, Act, Assert.
    netpod::V1_0::Server s;
}


TEST_F (ClientTest, CheckCreateClient)
{
    // Arrange, Act, Assert.
    netpod::V1_0::Client c;
}


TEST_F (ClientTest, ClientWrongAdressThrow)
{
    // Arrange.
    netpod::V1_0::ClientSettings settings;
    settings.addr = "abra-cadabra";
    netpod::V1_0::Client c(settings);

    // Act. Assert.
    EXPECT_THROW (c.request("something"), netpod::InvalidAddress);
}


TEST_F (ClientTest, ClientFailedToEstablishConnection)
{
    // Arrange 
    netpod::V1_0::ClientSettings settings;
    settings.addr = "LOOPBACK";
    netpod::V1_0::Client c(settings);

    // Act. Assert.
    EXPECT_THROW (c.request("something"), netpod::FailedToEstablishConnection);
}