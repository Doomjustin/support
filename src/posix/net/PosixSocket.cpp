#include "PosixSocket.h"
#include "../PosixUtils.h"
#include "support/Log.h"
#include "support/Utility.h"
#include "support/net/Socket.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>


namespace support::net::posix {

PosixSocket::PosixSocket(Domain domain, Type type)
  : Socket{ domain, type }
{
    if (!is_valid())
        open(domain, type);
}

PosixSocket::PosixSocket(int fd, Domain domain, Type type)
  : Socket{ domain, type }
{
    socket_ = fd;
    support::log::info("socket[{}] is built", socket_);
}

PosixSocket::~PosixSocket()
{
    if (is_valid())
        close();
}

void PosixSocket::open(Domain domain, Type type)
{
    socket_ = ::socket(support::posix::enum_cast(domain),
                       support::posix::enum_cast(type),
                       0);
    support::posix::check_result(socket_);

    support::log::info("socket[{}] opened", socket_);
}

void PosixSocket::close()
{
    auto res = ::close(socket_);
    support::posix::check_result(res);
    support::log::info("socket[{}] closed", socket_);
    socket_ = -1;
}

void PosixSocket::bind(const Endpoint& self)
{
    switch (domain_) {
    case support::net::Domain::IPv4:
    default:
        return bind_ipv4(self);
    case support::net::Domain::IPv6:
        return bind_ipv6(self);
    }
}

void PosixSocket::listen(int backlog)
{
    auto res = ::listen(socket_, backlog);
    support::posix::check_result(res);

    support::log::info("socket[{}] listening({})", socket_, backlog);
}

void PosixSocket::connect(const Endpoint& peer)
{
    switch (domain_) {
    case support::net::Domain::IPv4:
    default:
        return connect_ipv4(peer);
    case support::net::Domain::IPv6:
        return connect_ipv6(peer);
    }
}

std::pair<std::shared_ptr<Socket>, Endpoint> PosixSocket::accept()
{
    switch (domain_) {
    case support::net::Domain::IPv4:
    default:
        return accept_ipv4();
    case support::net::Domain::IPv6:
        return accept_ipv6();
    }
}

std::size_t PosixSocket::write(const void* buffer, std::size_t nbytes)
{
    auto res = ::write(socket_, buffer, nbytes);
    support::posix::check_result(res);

    support::log::info("socket[{}] writed {} bytes", socket_, res);
    return res;
}

std::size_t PosixSocket::read(void* buffer, std::size_t nbytes)
{
    auto res = ::read(socket_, buffer, nbytes);
    support::posix::check_result(res);

    support::log::info("socket[{}] readed {} bytes", socket_, res);
    return res;
}

void PosixSocket::keep_alive(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(socket_, SOL_SOCKET, SO_KEEPALIVE,
                 &optval, sizeof(optval));

    support::log::info("socket[{}] keep alive option {}", socket_, on);
}

void PosixSocket::address_reuse(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR,
                 &optval, sizeof(optval));

    support::log::info("socket[{}] address reuse option {}", socket_, on);
}

void PosixSocket::bind_ipv4(const Endpoint& self)
{
    auto addr = support::posix::cast_to_ipv4_addr(self);
    auto res = ::bind(socket_, (::sockaddr*) &addr, sizeof(addr));
    support::posix::check_result(res);
    support::log::info("socket[{}] binded on {}", socket_, support::to_string(self));
}

void PosixSocket::bind_ipv6(const Endpoint& self)
{
    auto addr = support::posix::cast_to_ipv6_addr(self);
    auto res = ::bind(socket_, (::sockaddr*) &addr, sizeof(addr));
    support::posix::check_result(res);
    support::log::info("socket[{}] binded on {}", socket_, support::to_string(self));
}

void PosixSocket::connect_ipv4(const Endpoint& peer)
{
    auto addr = support::posix::cast_to_ipv4_addr(peer);
    auto res = ::connect(socket_, (::sockaddr*) &addr, sizeof(addr));
    support::posix::check_result(res);
    support::log::info("socket[{}] connected to {}", socket_, support::to_string(peer));
}

void PosixSocket::connect_ipv6(const Endpoint& peer)
{
    auto addr = support::posix::cast_to_ipv6_addr(peer);
    auto res = ::connect(socket_, (::sockaddr*) &addr, sizeof(addr));
    support::posix::check_result(res);
    support::log::info("socket[{}] connected to {}", socket_, support::to_string(peer));
}

std::pair<std::shared_ptr<Socket>, Endpoint> PosixSocket::accept_ipv4()
{
    sockaddr_in addr{};
    socklen_t length{};
    auto res = ::accept(socket_, (::sockaddr*) &addr, &length);
    support::posix::check_result(res);

    auto peer_endpoint = support::posix::cast_to_endpoint(addr);
    auto peer_socket = std::make_shared<PosixSocket>(res, domain_, type_);

    support::log::info("socket[{}] accepted {} from {}",
                            socket_,
                            res,
                            support::to_string(peer_endpoint));
    return std::make_pair(peer_socket, peer_endpoint);
}

std::pair<std::shared_ptr<Socket>, Endpoint> PosixSocket::accept_ipv6()
{
    sockaddr_in6 addr{};
    socklen_t length{};
    auto res = ::accept(socket_, (::sockaddr*) &addr, &length);
    support::posix::check_result(res);

    auto peer_endpoint = support::posix::cast_to_endpoint(addr);
    auto peer_socket = std::make_shared<PosixSocket>(res, domain_, type_);

    support::log::info("socket[{}] accepted {} from {}",
                            socket_,
                            res,
                            support::to_string(peer_endpoint));
    return std::make_pair(peer_socket, peer_endpoint);
}

} // namespace support::net::posix