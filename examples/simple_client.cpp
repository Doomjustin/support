#include "support/net/Connector.h"
#include "support/net/Socket.h"

#include "fmt/core.h"

#include <cstdlib>


int main()
{
    using namespace support::net;

    Endpoint target{ "localhost", 12345 };
    Connector connector{ Domain::IPv4, Type::TCP };
    auto connection = connector.connect(target);
    connection->send("nothing to say");
    auto received = connection->receive(1024);
    fmt::print("{}\n", received);

    return EXIT_SUCCESS;
}