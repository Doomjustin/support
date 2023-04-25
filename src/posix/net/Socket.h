#ifndef CPP_SUPPORT_POSIX_NET_H
#define CPP_SUPPORT_POSIX_NET_H

#include "support/Net.h"


namespace support::net {

// 对Socket的简单包装，过于底层，不建议直接使用
class Socket: public Noncopyable {
public:
    Socket();
    Socket(Domain domain, Type type);

    Socket(Socket&& other) noexcept = default;
    Socket& operator=(Socket&& other) noexcept = default;

    ~Socket();

    constexpr Domain domain() const noexcept { return domain_; }
    constexpr Type type() const noexcept { return type_; }

    virtual void open() = 0;
    virtual void close() = 0;

    virtual std::unique_ptr<Socket> connect(const Endpoint& peer) = 0;

    virtual void bind(const Endpoint& self) = 0;
    virtual void listen(int backlog = 5) = 0;
    virtual std::unique_ptr<Socket> accept() = 0;

    virtual void reuse_address(bool on = true) = 0;
    virtual void keep_alive(bool on = true) = 0;

    virtual constexpr bool is_valid() const noexcept = 0;

protected:
    Domain domain_;
    Type type_;
};


// 对socket的wrapper，析构时自动关闭
class SocketGuard: public Noncopyable {
public:
    SocketGuard(std::unique_ptr<Socket> socket)
      : socket_{ std::move(socket) }
    {}

    SocketGuard(SocketGuard&&) noexcept = default;
    SocketGuard& operator=(SocketGuard&&) noexcept = default;

    ~SocketGuard()
    {
        socket_->close();
    }

    constexpr Domain domain() const noexcept { return socket_->domain(); }
    constexpr Type type() const noexcept { return socket_->type(); }

    std::unique_ptr<Socket> connect(const Endpoint& peer)
    {
        return socket_->connect(peer);
    }

    void bind(const Endpoint& self)
    {
        socket_->bind(self);
    }

    void listen(int backlog = 5)
    {
        socket_->listen(backlog);
    }

    std::unique_ptr<Socket> accept()
    {
        return socket_->accept();
    }

    void reuse_address(bool on = true)
    {
        socket_->reuse_address(on);
    }

    void keep_alive(bool on = true)
    {
        socket_->keep_alive(on);
    }

    constexpr bool is_valid() const noexcept
    {
        return socket_->is_valid();
    }

private:
    std::unique_ptr<Socket> socket_;
};

} // namespace support::posix::net

#endif // CPP_SUPPORT_POSIX_NET_H