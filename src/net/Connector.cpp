#include "Connector.h"
#include "Connection.h"
#include "Socket.h"
#include "support/Net.h"

#include <memory>


namespace support::net {

Connector::Connector(Domain domain, Type type)
  : IConnector{},
    socket_{ make_socket(domain, type) }
{}

std::unique_ptr<IConnection> Connector::connect(const Endpoint& peer)
{
    socket_->connect(peer);
    return std::make_unique<Connection>(std::move(socket_));
}

} // namespace support::net