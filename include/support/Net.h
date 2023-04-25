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
    explicit Endpoint(const std::string& host, std::uint16_t port = 0);
    explicit Endpoint(std::uint16_t port = 0);

    constexpr std::uint16_t port() const noexcept { return port_; }
    std::string_view host() const noexcept { return host_; }

    std::string to_string() const noexcept;

private:
    std::string host_;
    std::uint16_t port_;
};


// 双方建立连接后，负责收发数据
class Connection: public Noncopyable {
public:
    Connection() {}

    Connection(Connection&&) noexcept = default;
    Connection& operator=(Connection&&) noexcept = default;

    ~Connection() {}

    virtual constexpr bool is_valid() const noexcept = 0;
    virtual std::size_t read(void* buffer, std::size_t nbytes) = 0;
    virtual std::size_t write(const void* buffer, std::size_t nbytes) = 0;

    virtual constexpr Domain domain() const noexcept = 0;
    virtual constexpr Type type() const noexcept = 0;
};


// 作为客户端连接服务器
class Connector: public Noncopyable {
public:
    Connector() {}

    Connector(Connector&&) noexcept = default;
    Connector& operator=(Connector&&) noexcept = default;

    ~Connector() {}

    virtual std::unique_ptr<Connection> connect(const Endpoint& peer) = 0;
    virtual constexpr bool is_valid() const noexcept = 0;

    virtual constexpr Domain domain() const noexcept = 0;
    virtual constexpr Type type() const noexcept = 0;
};

std::unique_ptr<Connector> make_connector(Domain domain = Domain::IPv4, Type type = Type::TCP);


// 服务器等待客户端连接
class Acceptor: public Noncopyable {
public:
    Acceptor() {}

    Acceptor(Acceptor&&) noexcept = default;
    Acceptor& operator=(Acceptor&&) noexcept = default;

    ~Acceptor() {}

    virtual void bind(const Endpoint& self) = 0;
    virtual void listen(int backlog = 5) = 0;
    virtual std::unique_ptr<Connection> accept() = 0;
    virtual constexpr bool is_valid() const noexcept = 0;

    virtual void reuse_address() = 0;
    virtual void keep_alive() = 0;

    virtual constexpr Domain domain() const noexcept = 0;
    virtual constexpr Type type() const noexcept = 0;
};

std::unique_ptr<Acceptor> make_acceptor(Domain domain = Domain::IPv4, Type type = Type::TCP);


class SimpleClient: public Noncopyable {
public:
    SimpleClient(Domain domain, Type type, const Endpoint& peer)
      : state_{ ClientState::Initializing },
        peer_{ peer },
        connector_{ make_connector(domain, type) },
        connection_{}
    {}

    SimpleClient(SimpleClient&&) noexcept = default;
    SimpleClient& operator=(SimpleClient&&) noexcept = default;

    ~SimpleClient() {}

    void start()
    {
        connection_ = std::move(connector_->connect(peer_));

        begin();
        run();
        end();
    }

    constexpr Domain domain() const noexcept { return connector_->domain(); }
    constexpr Type type() const noexcept { return connector_->type(); }

protected:
    enum class ClientState {
        Initializing,
        Active,
        Finished
    };

protected:
    ClientState state_;
    Endpoint peer_;
    std::unique_ptr<Connector> connector_;
    std::unique_ptr<Connection> connection_;

    virtual void begin() {}
    virtual void end() {}
    // 通过实现ticks来处理业务逻辑，connection可以进行双方的通信
    virtual void ticks([[maybe_unused]] Connection& connection) {}

    // 可在ticks()中，调用以退出Client处理
    void finish() noexcept
    {
        state_ = ClientState::Finished;
    }

private:
    void run()
    {
        state_ = ClientState::Active;

        while (!finished()) {
            ticks(*connection_);
        }
    }

    constexpr bool finished() const noexcept { return state_ == ClientState::Finished; }
};

} // namespace support::net

#endif // CPP_SUPPORT_NET_H