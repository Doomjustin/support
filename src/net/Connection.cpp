#include "Connection.h"
#include "Socket.h"
#include "support/Net.h"


namespace support::net {

Connection::Connection(std::unique_ptr<Socket> socket)
  : IConnection{},
    socket_{ std::make_unique<SocketGuard>(std::move(socket)) }
{}

Connection::Connection(std::unique_ptr<SocketGuard> socket)
  : IConnection{},
    socket_{ std::move(socket) }
{}

std::size_t Connection::read(void* buffer, std::size_t nbytes)
{
    return socket_->read(buffer, nbytes);
}

std::size_t Connection::write(const void* buffer, std::size_t nbytes)
{
    return socket_->write(buffer, nbytes);
}

} // namespace support::net