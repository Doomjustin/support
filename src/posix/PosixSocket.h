#ifndef CPP_SUPPORT_POSIX_SOCKET_H
#define CPP_SUPPORT_POSIX_SOCKET_H

#include "../net/Socket.h"
#include "support/Net.h"


namespace support::posix {

class PosixSocket: public support::net::Socket {
public:
    PosixSocket(support::net::Domain domain, support::net::Type type);
    PosixSocket(int fd, support::net::Domain domain, support::net::Type type);

    PosixSocket(PosixSocket&&) noexcept = default;
    PosixSocket& operator=(PosixSocket&&) noexcept = default;

    ~PosixSocket() {}

    constexpr bool is_valid() const noexcept override
    {
        return fd_ != -1;
    }

    void open(support::net::Domain domain, support::net::Type type) override;
    void close() override;

    void connect(const support::net::Endpoint& peer) override;

    void bind(const support::net::Endpoint& self) override;
    void listen(int backlog = 5) override;
    support::net::SocketPtr accept() override;
    void reuse_address(bool on = true) override;
    void keep_alive(bool on = true) override;

    std::size_t read(void* buffer, std::size_t nbytes) override;
    std::size_t write(const void* buffer, std::size_t nbytes) override;

private:
    int fd_;

    void connect_ipv4(const support::net::Endpoint& peer);
    void connect_ipv6(const support::net::Endpoint& peer);

    void bind_ipv4(const support::net::Endpoint& self);
    void bind_ipv6(const support::net::Endpoint& self);

    support::net::SocketPtr accept_ipv4();
    support::net::SocketPtr accept_ipv6();
};

} // namespace support::posix

#endif // CPP_SUPPORT_POSIX_SOCKET_H