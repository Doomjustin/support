#ifndef CPP_SUPPORT_NET_SERVER_H
#define CPP_SUPPORT_NET_SERVER_H

#include "Connection.h"
#include "Acceptor.h"
#include "support/net/Endpoint.h"

#include <memory>


namespace support::net {

// 迭代式服务器，一次只能处理一个连接
class BasicServer {
public:
    BasicServer(Domain domain = Domain::IPv4, Type type = Type::TCP);

    ~BasicServer();

    void start();
    void stop();

protected:
    virtual void on_connected(std::shared_ptr<Connection> connection) {}

    virtual void on_start() {}
    virtual void on_stop() {}

    enum class ServerState {
        Init,
        Pending,
        Active,
        Exit
    };

protected:
    std::unique_ptr<Acceptor> acceptor_;
    ServerState state_;
    Endpoint self_;
    int listen_backlog_ = 5;

private:
    void run();

    constexpr bool should_stop() const noexcept { return state_ == ServerState::Exit; }
};

} // namespace support::net

#endif // CPP_SUPPORT_NET_SERVER_H