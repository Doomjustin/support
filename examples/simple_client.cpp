#include "support/net/Connector.h"
#include "support/net/Endpoint.h"
#include "support/net/Socket.h"
#include "support/net/Client.h"

#include "fmt/core.h"

#include <cstdlib>
#include <iostream>
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
        bool active = true;
        while (active) {
            std::string in{};
            std::cin >> in;
            connection->send(in);
            auto received = connection->receive(1024);
            std::cout << ">> " << received << std::endl;

            if (received == "quit")
                active = false;
        }
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