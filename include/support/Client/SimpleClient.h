#ifndef CPP_SUPPORT_CLIENT_SIMPLE_CLIENT_H
#define CPP_SUPPORT_CLIENT_SIMPLE_CLIENT_H

#include "../Net.h"


namespace support::net {

// 基础的客户端
class SimpleClient: public Noncopyable {
public:
    SimpleClient(Domain domain, Type type, const Endpoint& peer);

    SimpleClient(SimpleClient&&) noexcept = default;
    SimpleClient& operator=(SimpleClient&&) noexcept = default;

    ~SimpleClient() {}

    void start();

    Domain domain() const noexcept { return connector_->domain(); }
    Type type() const noexcept { return connector_->type(); }

protected:
    enum class ClientState {
        Initializing,
        Active,
        Finished
    };

protected:
    ClientState state_;
    Endpoint peer_;
    std::unique_ptr<IConnector> connector_;
    std::unique_ptr<IConnection> connection_;

    virtual void begin() {}
    virtual void end() {}
    // 通过实现ticks来处理业务逻辑，connection可以进行双方的通信
    virtual void ticks([[maybe_unused]] IConnection& connection) {}

    // 可在ticks()中，调用以退出Client处理
    void finish() noexcept;

private:
    void run();

    constexpr bool finished() const noexcept
    {
        return state_ == ClientState::Finished;
    }
};

} // namespace support::net

#endif // CPP_SUPPORT_CLIENT_SIMPLE_CLIENT_H