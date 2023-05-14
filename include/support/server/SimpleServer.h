#ifndef CPP_SUPPORT_SERVER_SIMPLE_SERVER_H
#define CPP_SUPPORT_SERVER_SIMPLE_SERVER_H

#include "../Net.h"


namespace support::net {

// 一次仅能处理一个连接的简单服务器
class SimpleServer {
public:
    SimpleServer(Domain domain, Type type, const Endpoint& self);

    SimpleServer(SimpleServer&&) noexcept = default;
    SimpleServer& operator=(SimpleServer&&) noexcept = default;

    ~SimpleServer() {}

    void start();

    Domain domain() const noexcept { return acceptor_->domain(); }
    Type type() const noexcept { return acceptor_->type(); }

protected:
    enum class ServerState {
        Initializing,
        Active,
        Finished
    };

protected:
    ServerState state_;
    Endpoint self_;
    std::unique_ptr<IAcceptor> acceptor_;

    virtual void begin() {}
    virtual void end() {}
    // 通过实现on_connect来处理一条连接的业务，connection可以进行双方的通信
    virtual void on_connect([[maybe_unused]] IConnection& connection) {}

    // 可在on_connect()中，调用以退出Client处理
    void finish() noexcept;

private:
    void run();

    constexpr bool finished() const noexcept
    {
        return state_ == ServerState::Finished;
    }
};

} // namespace support::net

#endif // CPP_SUPPORT_SERVER_SIMPLE_SERVER_H