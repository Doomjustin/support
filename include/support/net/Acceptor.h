#ifndef CPP_SUPPORT_NET_ACCEPTOR_H
#define CPP_SUPPORT_NET_ACCEPTOR_H

#include "Socket.h"
#include "Connection.h"
#include "Endpoint.h"

#include <memory>


namespace support::net {

class Acceptor {
public:
    explicit Acceptor(Domain domain = Domain::IPv4, Type type = Type::TCP);
    explicit Acceptor(std::shared_ptr<Socket> socket);

    void bind(const Endpoint& self);

    void listen(int backlog = 5);

    std::shared_ptr<Connection> accept();

    std::shared_ptr<Socket> socket() const noexcept { return socket_; }

    bool is_valid() const noexcept { return socket_->is_valid(); }

    void keep_alive(bool on) { socket_->keep_alive(on); }
    void address_reuse(bool on) { socket_->address_reuse(on); }

private:
    std::shared_ptr<Socket> socket_;
};

} // namespace support::net

#endif // CPP_SUPPORT_NET_ACCEPTOR_H