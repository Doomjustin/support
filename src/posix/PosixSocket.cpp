#include "PosixSocket.h"
#include "PosixUtils.h"
#include "support/Net.h"

#include <fcntl.h>
#include <memory>
#include <sys/socket.h>
#include <unistd.h>


namespace support::posix {

PosixSocket::PosixSocket(support::net::Domain domain, support::net::Type type)
  : Socket{ domain, type }, fd_{ -1 }
{
    open(domain, type);
}

PosixSocket::PosixSocket(int fd, support::net::Domain domain, support::net::Type type)
  : Socket{ domain, type }, fd_{ fd }
{}

void PosixSocket::open(support::net::Domain domain, support::net::Type type)
{
    fd_ = ::socket(enum_cast(domain), enum_cast(type), 0);
    check_result(fd_);
}

void PosixSocket::close()
{
    auto res = ::close(fd_);
    check_result(res);
    fd_ = -1;
}

void PosixSocket::connect(const support::net::Endpoint& peer)
{
    switch (domain()) {
    case support::net::Domain::IPv4:
    default:
        return connect_ipv4(peer);
    case support::net::Domain::IPv6:
        return connect_ipv6(peer);
    }
}

void PosixSocket::bind(const support::net::Endpoint& self)
{
    switch (domain()) {
    case support::net::Domain::IPv4:
    default:
        return bind_ipv4(self);
    case support::net::Domain::IPv6:
        return bind_ipv6(self);
    }
}

void PosixSocket::listen(int backlog)
{
    auto res = ::listen(fd_, backlog);
    check_result(res);
}

support::net::SocketPtr PosixSocket::accept()
{
    switch (domain()) {
    case support::net::Domain::IPv4:
    default:
        return accept_ipv4();
    case support::net::Domain::IPv6:
        return accept_ipv6();
    }
}

void PosixSocket::reuse_address(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR,
                 &optval, sizeof(optval));
}

void PosixSocket::keep_alive(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE,
                 &optval, sizeof(optval));
}

std::size_t PosixSocket::read(void* buffer, std::size_t nbytes)
{
    auto read_bytes = ::read(fd_, buffer, nbytes);
    check_result(read_bytes);

    return read_bytes;
}

std::size_t PosixSocket::write(const void* buffer, std::size_t nbytes)
{
    auto write_bytes = ::write(fd_, buffer, nbytes);
    check_result(write_bytes);

    return write_bytes;
}

void PosixSocket::bind_ipv4(const support::net::Endpoint& self)
{
    auto addr = cast_to_ipv4_addr(self);
    auto res = ::bind(fd_, (::sockaddr*) &addr, sizeof(addr));
    check_result(res);
}

void PosixSocket::bind_ipv6(const support::net::Endpoint& self)
{
    auto addr = cast_to_ipv6_addr(self);
    auto res = ::bind(fd_, (::sockaddr*) &addr, sizeof(addr));
    check_result(res);
}

void PosixSocket::connect_ipv4(const support::net::Endpoint& peer)
{
    auto addr = cast_to_ipv4_addr(peer);
    auto res = ::connect(fd_, (::sockaddr*) &addr, sizeof(addr));
    check_result(res);
}

void PosixSocket::connect_ipv6(const support::net::Endpoint& peer)
{
    auto addr = cast_to_ipv6_addr(peer);
    auto res = ::connect(fd_, (::sockaddr*) &addr, sizeof(addr));
    check_result(res);
}

support::net::SocketPtr PosixSocket::accept_ipv4()
{
    sockaddr_in addr{};
    socklen_t length{};
    auto res = ::accept(fd_, (::sockaddr*) &addr, &length);
    check_result(res);

    auto peer_endpoint = cast_to_endpoint(addr);
    support::net::SocketPtr peer_socket{ new PosixSocket{ res, domain_, type_ },
                                         support::net::SocketDeleter{} };

    return peer_socket;
}

support::net::SocketPtr PosixSocket::accept_ipv6()
{
    sockaddr_in6 addr{};
    socklen_t length{};
    auto res = ::accept(fd_, (::sockaddr*) &addr, &length);
    check_result(res);

    auto peer_endpoint = cast_to_endpoint(addr);
    support::net::SocketPtr peer_socket{ new PosixSocket{ res, domain_, type_ },
                                         support::net::SocketDeleter{} };

    return peer_socket;
}

} // namespace support::posix