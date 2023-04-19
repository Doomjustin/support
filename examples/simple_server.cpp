#include "support/Log.h"
#include "support/net/Acceptor.h"
#include "support/net/Connection.h"
#include "support/net/Endpoint.h"
#include "support/net/Server.h"

#include <cstdlib>
#include <memory>

using namespace support::net;

class EchoServer: public BasicServer {
public:
    EchoServer(const Endpoint& self)
      : BasicServer{ self }
    {}

    ~EchoServer() {}

    void on_connected(std::shared_ptr<Connection> connction) override
    {
        auto received_ = connction->receive(1024);
        connction->send(received_);
    }

    void on_start() override
    {
        support::log::info("echo server started");
    }

    void on_stop() override
    {
        support::log::info("echo server stopped");
    }
};

int main(int argc, char* argv[])
{
    // Acceptor acceptor{};
    // Endpoint self{ 12345 };
    // acceptor.bind(self);
    // acceptor.listen();

    // while (true) {
    //     auto connection = acceptor.accept();
    //     auto received = connection->receive(1024);
    //     connection->send("hello world");
    // }

    EchoServer server{ Endpoint{ 12345 } };
    server.start();

    return EXIT_SUCCESS;
}