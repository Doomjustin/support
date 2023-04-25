#ifndef CPP_SUPPORT_NET_CONNECTION_H
#define CPP_SUPPORT_NET_CONNECTION_H

#include "support/Net.h"
#include "Socket.h"

#include <memory>


namespace support::net {

class Connection: public IConnection {
public:
    Connection(SocketPtr socket);

    Connection(Connection&&) noexcept = default;
    Connection& operator=(Connection&&) noexcept = default;

    ~Connection() {}

    std::size_t read(void* buffer, std::size_t nbytes) override;
    std::size_t write(const void* buffer, std::size_t nbytes) override;

    bool is_valid() const noexcept override
    {
        return socket_->is_valid();
    }

    Domain domain() const noexcept override
    {
        return socket_->domain();
    }

    Type type() const noexcept override
    {
        return socket_->type();
    }

private:
    SocketPtr socket_;
};

} // namespace support::net

#endif // CPP_SUPPORT_NET_CONNECTION_H