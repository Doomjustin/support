#ifndef CPP_SUPPORT_POSIX_NET_H
#define CPP_SUPPORT_POSIX_NET_H

#include "support/Net.h"

#include <memory>


namespace support::net {

class SocketDeleter;
class Socket;
using SocketPtr = std::unique_ptr<Socket, SocketDeleter>;

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
    virtual SocketPtr accept() = 0;

    virtual void reuse_address(bool on = true) = 0;
    virtual void keep_alive(bool on = true) = 0;

    virtual std::size_t read(void* buffer, std::size_t nbytes) = 0;
    virtual std::size_t write(const void* buffer, std::size_t nbytes) = 0;

protected:
    Domain domain_;
    Type type_;
};

class SocketDeleter {
public:
    void operator()(Socket* socket)
    {
        socket->close();
    }
};

SocketPtr make_socket(Domain domain, Type type);

} // namespace support::net

#endif // CPP_SUPPORT_POSIX_NET_H