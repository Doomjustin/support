#include "support/net/Connector.h"
#include "support/net/Connection.h"
#include "support/net/Socket.h"


namespace support::net {

Connector::Connector(Domain domain, Type type)
  : socket_{ open(domain, type) }
{}

Connector::Connector(std::shared_ptr<Socket> socket)
  : socket_{ socket }
{}

std::shared_ptr<Connection> Connector::connect(const Endpoint& peer)
{
    socket_->connect(peer);

    return std::make_shared<Connection>(socket_);
}

} // namespace support::net