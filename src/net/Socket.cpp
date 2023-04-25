#include "Socket.h"
#include "../posix/PosixSocket.h"

#include <memory>


namespace support::net {

std::unique_ptr<Socket> make_socket(Domain domain, Type type)
{
    return std::make_unique<support::posix::PosixSocket>(domain, type);
}

SocketGuard::SocketGuard(Domain domain, Type type)
  : socket_{ make_socket(domain, type) }
{}

SocketGuard::SocketGuard(std::unique_ptr<Socket> socket)
  : socket_{ std::move(socket) }
{}

SocketGuard::~SocketGuard()
{
    socket_->close();
}

void SocketGuard::connect(const Endpoint& peer)
{
    return socket_->connect(peer);
}

void SocketGuard::bind(const Endpoint& self)
{
    socket_->bind(self);
}

void SocketGuard::listen(int backlog)
{
    socket_->listen(backlog);
}

std::unique_ptr<Socket> SocketGuard::accept()
{
    return socket_->accept();
}

void SocketGuard::reuse_address(bool on)
{
    socket_->reuse_address(on);
}

void SocketGuard::keep_alive(bool on)
{
    socket_->keep_alive(on);
}

std::size_t SocketGuard::read(void* buffer, std::size_t nbytes)
{
    return socket_->read(buffer, nbytes);
}

std::size_t SocketGuard::write(const void* buffer, std::size_t nbytes)
{
    return socket_->write(buffer, nbytes);
}

} // namespace support::net