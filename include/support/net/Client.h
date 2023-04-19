#ifndef CPP_SUPPORT_NET_CLIENT_H
#define CPP_SUPPORT_NET_CLIENT_H

#include "Connector.h"
#include "Connection.h"
#include "support/net/Endpoint.h"

#include <memory>


namespace support::net {

class BasicClient {
public:
    explicit BasicClient(Domain domain = Domain::IPv4, Type type = Type::TCP);
    explicit BasicClient(Endpoint peer, Domain domain = Domain::IPv4, Type type = Type::TCP);

    ~BasicClient();

    void start();
    void stop();

protected:
    virtual void on_connected(std::shared_ptr<Connection> connection) {}
    virtual void on_start() {}
    virtual void on_stop() {}

protected:
    std::unique_ptr<Connector> acceptor_;
    Endpoint peer_;
    std::shared_ptr<Connection> connection_;

    void run();
};

} // namespace support::net

#endif // CPP_SUPPORT_NET_CLIENT_H