#include "support/net/Acceptor.h"
#include "support/net/Connection.h"

#include <memory>


namespace support::net {

Acceptor::Acceptor(Domain domain, Type type)
  : socket_{ open(domain, type) }
{}

Acceptor::Acceptor(std::shared_ptr<Socket> socket)
  : socket_{ socket }
{}

void Acceptor::bind(const Endpoint& self)
{
    socket_->bind(self);
}

void Acceptor::listen(int backlog)
{
    socket_->listen(backlog);
}

std::shared_ptr<Connection> Acceptor::accept()
{
    auto [peer_socket, peer_endpoint] = socket_->accept();

    return std::make_shared<Connection>(peer_socket);
}

} // namespace support::net