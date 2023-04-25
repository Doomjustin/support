#include "Acceptor.h"
#include "Socket.h"
#include "Connection.h"

#include <memory>


namespace support::net {

Acceptor::Acceptor(Domain domain, Type type)
  : socket_{ make_socket(domain, type) }
{}

void Acceptor::bind(const Endpoint& self)
{
    socket_->bind(self);
}

void Acceptor::listen(int backlog)
{
    socket_->listen(backlog);
}

std::unique_ptr<IConnection> Acceptor::accept()
{
    auto peer_socket = socket_->accept();

    return std::make_unique<Connection>(std::move(peer_socket));
}

void Acceptor::reuse_address(bool on)
{
    socket_->reuse_address(on);
}

void Acceptor::keep_alive(bool on)
{
    socket_->keep_alive(on);
}

} // namespace support::net