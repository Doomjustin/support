#include "support/Net.h"

#include <cstdlib>
#include <iostream>


using namespace support::net;


class EchoServer: public SimpleServer {
public:
    EchoServer(Domain domain, Type type, const Endpoint& self)
      : SimpleServer{ domain, type, self }
    {}

    ~EchoServer() {}

    void on_connect(IConnection& connection) override
    {
        std::cout << "new connection came" << "\n";
        char buffer[4096];
        auto read_bytes = connection.read(buffer, 4096);
        // 将所有数据都反写回客户端
        while (read_bytes != 0) {
            std::cout << "received: " << buffer << std::endl;
            connection.write(buffer, read_bytes);
            read_bytes = connection.read(buffer, 4096);
        }
    }
};


int main(int argc, char* argv[])
{
    const Endpoint self_address{ 12345 };
    EchoServer server{ Domain::IPv4, Type::TCP, self_address };
    server.start();

    return EXIT_SUCCESS;
}