#ifndef CPP_SUPPORT_SERVER_MULTI_SERVER_H
#define CPP_SUPPORT_SERVER_MULTI_SERVER_H

#include "Worker.h"
#include "support/Net.h"

#include <cstdint>
#include <iostream>
#include <memory>
#include <functional>
#include <thread>
#include <vector>


namespace support::net {

// 多线程
class MultiThreadServer {
public:
    MultiThreadServer(const Endpoint& self, Domain domain = Domain::IPv4, Type type = Type::TCP);

    ~MultiThreadServer() {}

    // 用户需要实现on_read和on_except来实现业务逻辑
    virtual void on_read(IConnection& connection)
    {
        std::cout << "called" << std::endl;
    }

    virtual void on_except(IConnection& connection) {}

    void start();

private:
    Endpoint self_;
    std::unique_ptr<IAcceptor> acceptor_;
    std::vector<std::unique_ptr<Worker>> connection_managers_;

    static std::uint8_t max_threads_;  // 16

    // 适合的worker来放置新的connection -> 空闲或者连接数最少的worker
    Worker* get_available_worker();

    void create_new_worker();
};

} // namespace support::net

#endif // CPP_SUPPORT_SERVER_MULTI_SERVER_H