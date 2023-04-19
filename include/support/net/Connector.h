#ifndef CPP_SUPPORT_NET_CONNECTOR_H
#define CPP_SUPPORT_NET_CONNECTOR_H

#include "Socket.h"
#include "Endpoint.h"
#include "Connection.h"

#include <memory>


namespace support::net {

class Connector {
public:
    explicit Connector(Domain domain = Domain::IPv4, Type type = Type::TCP);

    explicit Connector(std::shared_ptr<Socket> socket);

    std::shared_ptr<Connection> connect(const Endpoint& peer);

    std::shared_ptr<Socket> socket() const noexcept { return socket_; }

private:
    std::shared_ptr<Socket> socket_;
};

} // namespace support::net

#endif // CPP_SUPPORT_NET_CONNECTOR_H