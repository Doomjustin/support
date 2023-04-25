#ifndef CPP_SUPPORT_NET_H
#define CPP_SUPPORT_NET_H

#include "support/Noncopyable.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <memory>


namespace support::net {

enum class Domain {
    IPv4,
    IPv6
};

enum class Type {
    TCP,
    UDP
};


class Endpoint {
public:
    explicit Endpoint(const std::string& host, std::uint16_t port = 0)
      : host_{ host }, port_{ port }
    {}

    explicit Endpoint(std::uint16_t port = 0)
      : host_{}, port_{ port }
    {}

    constexpr std::uint16_t port() const noexcept { return port_; }
    std::string_view host() const noexcept { return host_; }

    // std::string to_string() const noexcept;

private:
    std::string host_;
    std::uint16_t port_;
};


// 双方建立连接后，负责收发数据
class IConnection: public Noncopyable {
public:
    IConnection() {}

    IConnection(IConnection&&) noexcept = default;
    IConnection& operator=(IConnection&&) noexcept = default;

    ~IConnection() {}

    virtual std::size_t read(void* buffer, std::size_t nbytes) = 0;
    virtual std::size_t write(const void* buffer, std::size_t nbytes) = 0;

    virtual bool is_valid() const noexcept = 0;
    virtual Domain domain() const noexcept = 0;
    virtual Type type() const noexcept = 0;
};


// 作为客户端连接服务器
class IConnector: public Noncopyable {
public:
    IConnector() {}

    IConnector(IConnector&&) noexcept = default;
    IConnector& operator=(IConnector&&) noexcept = default;

    ~IConnector() {}

    virtual std::unique_ptr<IConnection> connect(const Endpoint& peer) = 0;

    virtual bool is_valid() const noexcept = 0;
    virtual Domain domain() const noexcept = 0;
    virtual Type type() const noexcept = 0;
};

std::unique_ptr<IConnector> make_connector(Domain domain = Domain::IPv4, Type type = Type::TCP);


// 服务器等待客户端连接
class IAcceptor: public Noncopyable {
public:
    IAcceptor() {}

    IAcceptor(IAcceptor&&) noexcept = default;
    IAcceptor& operator=(IAcceptor&&) noexcept = default;

    ~IAcceptor() {}

    virtual void bind(const Endpoint& self) = 0;
    virtual void listen(int backlog = 5) = 0;
    virtual std::unique_ptr<IConnection> accept() = 0;

    virtual void reuse_address(bool on = true) = 0;
    virtual void keep_alive(bool on = true) = 0;

    virtual bool is_valid() const noexcept = 0;
    virtual Domain domain() const noexcept = 0;
    virtual Type type() const noexcept = 0;
};

std::unique_ptr<IAcceptor> make_acceptor(Domain domain = Domain::IPv4, Type type = Type::TCP);


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

    // 可在ticks()中，调用以退出Client处理
    void finish() noexcept;

private:
    void run();

    constexpr bool finished() const noexcept
    {
        return state_ == ServerState::Finished;
    }
};

} // namespace support::net

#endif // CPP_SUPPORT_NET_H