#include "support/Net.h"

#include <cstdlib>
#include <iostream>

using namespace support::net;

class EchoClient: public SimpleClient {
public:
    EchoClient(Domain domain, Type type, const Endpoint& peer)
      : SimpleClient{ domain, type, peer }
    {}

    ~EchoClient() {}

    void ticks(IConnection& connection) override
    {
        std::string getin;

        std::cin >> getin;

        connection.write(getin.c_str(), getin.size());

        char buffer[4096];
        auto read_bytes = connection.read(buffer, 4096);
        buffer[read_bytes] = 0;
        std::cout << buffer << "\n";

        if (getin == "quit")
            finish();

    }
};


int main(int argc, char* argv[])
{
    const Endpoint peer_endpoint{ 12345 };
    EchoClient client{ Domain::IPv4, Type::TCP, peer_endpoint };

    client.start();

    return EXIT_SUCCESS;
}