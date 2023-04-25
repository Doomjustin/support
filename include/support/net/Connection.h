#ifndef CPP_SUPPORT_NET_CONNECTION_H
#define CPP_SUPPORT_NET_CONNECTION_H

#include "support/net/Endpoint.h"
#include "support/net/Socket.h"

#include <cstddef>
#include <memory>
#include <type_traits>


namespace support::net {

// 对socket的wrapper，封装了io方法
class Connection {
public:
    Connection(std::shared_ptr<Socket> socket);

    ~Connection() {}

    std::size_t read(void* buffer, std::size_t length);

    std::size_t write(const void* buffer, std::size_t length);

    void send(const std::string& value);

    std::string receive(std::size_t size);

    std::shared_ptr<Socket> socket() const noexcept { return socket_; }

    bool is_valid() const noexcept { return socket_->is_valid(); }

protected:
    std::shared_ptr<net::Socket> socket_;
};

} // namespace support::net

#endif // CPP_SUPPORT_NET_CONNECTION_H