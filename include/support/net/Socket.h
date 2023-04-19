#ifndef CPP_SUPPORT_NET_SOCKET_H
#define CPP_SUPPORT_NET_SOCKET_H

#include "support/Noncopyable.h"
#include "support/net/Endpoint.h"

#include <memory>
#include <utility>


namespace support::net {

enum class Domain {
    IPv4,
    IPv6
};

enum class Type {
    TCP,
    UDP
};

// 析构时close
class Socket: public Noncopyable {
public:
    Socket(Domain domain, Type type);

    Socket(Socket&&) noexcept = default;
    Socket& operator=(Socket&&) noexcept = default;

    ~Socket() {}

    constexpr bool is_valid() const noexcept { return socket_ != -1; };
    constexpr int native_handler() const noexcept { return socket_; };

    virtual void bind(const Endpoint& self) = 0;
    virtual void listen(int backlog) = 0;
    virtual void connect(const Endpoint& peer) = 0;
    virtual std::pair<std::shared_ptr<Socket>, Endpoint> accept() = 0;

    virtual std::size_t write(const void* buffer, std::size_t nbytes) = 0;
    virtual std::size_t read(void* buffer, std::size_t nbytes) = 0;

    virtual void keep_alive(bool on) = 0;
    virtual void address_reuse(bool on) = 0;

protected:
    int socket_;
    Domain domain_;
    Type type_;
};

std::shared_ptr<Socket> open(Domain domain, Type type);

} // namespace support::net

#endif // CPP_SUPPORT_NET_SOCKET_H