#ifndef CPP_SUPPORT_NET_CONNECTION_H
#define CPP_SUPPORT_NET_CONNECTION_H

#include "support/net/Endpoint.h"
#include "support/net/Socket.h"

#include <memory>
#include <type_traits>


namespace support::net {

// 对socket的wrapper，封装了io方法
class Connection {
public:
    Connection(std::shared_ptr<Socket> socket);

    ~Connection() {}

    void send(const std::string& value);

    std::string receive(std::size_t size);

protected:
    std::shared_ptr<net::Socket> socket_;
};

} // namespace support::net

#endif // CPP_SUPPORT_NET_CONNECTION_H