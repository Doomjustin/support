#ifndef CPP_SUPPORT_POSIX_NET_H
#define CPP_SUPPORT_POSIX_NET_H

#include "support/net/Endpoint.h"
#include "support/net/Socket.h"


namespace support::net::posix {

class PosixSocket: public Socket {
public:
    PosixSocket(Domain domain, Type type);
    PosixSocket(int fd, Domain domain, Type type);

    ~PosixSocket();

    void bind(const Endpoint& self) override;
    void listen(int backlog) override;
    void connect(const Endpoint& peer) override;
    std::pair<std::shared_ptr<Socket>, Endpoint> accept() override;
    std::size_t write(const void* buffer, std::size_t nbytes) override;
    std::size_t read(void* buffer, std::size_t nbytes) override;
    void keep_alive(bool on) override;
    void address_reuse(bool on) override;

    void open(Domain domain, Type type);
    void close();

private:
    void bind_ipv4(const Endpoint& self);
    void bind_ipv6(const Endpoint& self);

    void connect_ipv4(const Endpoint& peer);
    void connect_ipv6(const Endpoint& peer);

    std::pair<std::shared_ptr<Socket>, Endpoint> accept_ipv4();
    std::pair<std::shared_ptr<Socket>, Endpoint> accept_ipv6();
};

} // namespace support::net::posix

#endif // CPP_SUPPORT_POSIX_NET_H