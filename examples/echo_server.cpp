#include "support/Net.h"
#include "support/Server/MultiServer.h"

#include <cstdlib>
#include <iostream>
#include <ostream>


using namespace support::net;

class EchoServer: public MultiThreadServer {
public:
    EchoServer(const Endpoint& self, Domain domain = Domain::IPv4, Type type = Type::TCP)
      : MultiThreadServer{ self, domain, type }
    {}

    ~EchoServer() {}

    void on_read(IConnection& connection) override
    {
        char buffer[4096];
        auto read_bytes = connection.read(buffer, 4096);
        if (read_bytes == 0) {
            connection.close();
            return;
        }

        buffer[read_bytes] = 0;
        std::cout << "read from client: " << buffer << "\n";
        connection.write(buffer, read_bytes);
    }

    void on_except(IConnection& connection) override
    {
        // do nothing
        std::cout << "error happened" << std::endl;
    }

};


int main(int argc, char* argv[])
{
    const Endpoint self_address{ 12345 };

    EchoServer echo_server{ self_address };
    echo_server.start();

    return EXIT_SUCCESS;
}