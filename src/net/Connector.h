#ifndef CPP_SUPPORT_NET_CONNECTOR_H
#define CPP_SUPPORT_NET_CONNECTOR_H

#include "Socket.h"
#include "support/Net.h"

#include <memory>


namespace support::net {

class Connector: public IConnector {
public:
    Connector(Domain domain, Type type);

    Connector(Connector&&) noexcept = default;
    Connector& operator=(Connector&&) noexcept = default;

    ~Connector() {}

    std::unique_ptr<IConnection> connect(const Endpoint& peer) override;

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
    std::unique_ptr<SocketGuard> socket_;
};

} // namespace support::net

#endif // CPP_SUPPORT_NET_CONNECTOR_H