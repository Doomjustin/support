#include "support/net/Connector.h"
#include "support/net/Endpoint.h"
#include "support/net/Socket.h"
#include "support/net/Client.h"

#include "fmt/core.h"

#include <cstdlib>
#include <memory>

using namespace support::net;

class EchoClient: public BasicClient {
public:
    EchoClient(const Endpoint& peer)
      : BasicClient{ peer }
    {}

    ~EchoClient() {}

    void on_connected(std::shared_ptr<Connection> connection) override
    {
        connection->send("hello world");
        auto received = connection->receive(1024);
        fmt::print("received from server: {}\n", received);
    }
};


int main()
{
    Endpoint target{ "localhost", 12345 };
    // Connector connector{ Domain::IPv4, Type::TCP };
    // auto connection = connector.connect(target);
    // connection->send("nothing to say");
    // auto received = connection->receive(1024);
    // fmt::print("{}\n", received);

    EchoClient client{ target };
    client.start();

    return EXIT_SUCCESS;
}