#ifndef CPP_SUPPORT_POSIX_NET_H
#define CPP_SUPPORT_POSIX_NET_H

#include "support/Net.h"

#include <memory>


namespace support::net {

// 对Socket的简单包装，过于底层，用户不应该直接使用
class Socket: public Noncopyable {
public:
    Socket() {}

    Socket(Domain domain, Type type)
      : domain_{ domain }, type_{ type }
    {}

    Socket(Socket&& other) noexcept = default;
    Socket& operator=(Socket&& other) noexcept = default;

    ~Socket() {}

    constexpr Domain domain() const noexcept { return domain_; }
    constexpr Type type() const noexcept { return type_; }
    virtual constexpr bool is_valid() const noexcept = 0;

    virtual void open(Domain domain, Type type) = 0;
    virtual void close() = 0;

    virtual void connect(const Endpoint& peer) = 0;

    virtual void bind(const Endpoint& self) = 0;
    virtual void listen(int backlog = 5) = 0;
    virtual std::unique_ptr<Socket> accept() = 0;

    virtual void reuse_address(bool on = true) = 0;
    virtual void keep_alive(bool on = true) = 0;

    virtual std::size_t read(void* buffer, std::size_t nbytes) = 0;
    virtual std::size_t write(const void* buffer, std::size_t nbytes) = 0;

protected:
    Domain domain_;
    Type type_;
};

std::unique_ptr<Socket> make_socket(Domain domain, Type type);


// 对socket的wrapper，析构时自动关闭
class SocketGuard: public Noncopyable {
public:
    SocketGuard(std::unique_ptr<Socket> socket);
    SocketGuard(Domain domain, Type type);

    SocketGuard(SocketGuard&&) noexcept = default;
    SocketGuard& operator=(SocketGuard&&) noexcept = default;

    ~SocketGuard();

    Domain domain() const noexcept
    {
        return socket_->domain();
    }

    Type type() const noexcept
    {
        return socket_->type();
    }

    void connect(const Endpoint& peer);

    void bind(const Endpoint& self);
    void listen(int backlog = 5);
    std::unique_ptr<Socket> accept();

    void reuse_address(bool on = true);
    void keep_alive(bool on = true);

    bool is_valid() const noexcept
    {
        return static_cast<bool>(socket_) && socket_->is_valid();
    }

    std::size_t read(void* buffer, std::size_t nbytes);
    std::size_t write(const void* buffer, std::size_t nbytes);

    // 释放管理
    std::unique_ptr<Socket> release() noexcept { return std::move(socket_); }

private:
    std::unique_ptr<Socket> socket_;
};

} // namespace support::net

#endif // CPP_SUPPORT_POSIX_NET_H