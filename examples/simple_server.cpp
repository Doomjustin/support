#include "support/net/Acceptor.h"
#include "support/net/Endpoint.h"

#include <cstdlib>


int main(int argc, char* argv[])
{
    using namespace support::net;

    Acceptor acceptor{};
    Endpoint self{ 12345 };
    acceptor.bind(self);
    acceptor.listen();

    while (true) {
        auto connection = acceptor.accept();
        auto received = connection->receive(1024);
        connection->send("hello world");
    }

    return EXIT_SUCCESS;
}