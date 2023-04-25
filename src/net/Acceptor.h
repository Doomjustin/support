#ifndef CPP_SUPPORT_NET_ACCEPTOR_H
#define CPP_SUPPORT_NET_ACCEPTOR_H

#include "support/Net.h"
#include "Socket.h"

#include <memory>


namespace support::net {

class Acceptor: public IAcceptor {
public:
    Acceptor(Domain domain, Type type);

    Acceptor(Acceptor&&) noexcept = default;
    Acceptor& operator=(Acceptor&&) noexcept = default;

    ~Acceptor() {}

    void bind(const Endpoint& self) override;
    void listen(int backlog = 5) override;
    std::unique_ptr<IConnection> accept() override;

    void reuse_address(bool on = true) override;
    void keep_alive(bool on = true) override;

    bool is_valid() const noexcept override
    {
        return socket_->is_valid();
    }

    Domain domain() const noexcept override
    {
        return socket_->domain();
    }

    Type type() const noexcept override
    {
        return socket_->type();
    }

private:
    SocketPtr socket_;
};

} // namespace support::net

#endif // CPP_SUPPORT_NET_ACCEPTOR_H