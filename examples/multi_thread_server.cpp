#include "support/Net.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <map>

#include <sys/select.h>
#include <thread>


using namespace support::net;

class Selector {
public:
    using ReadCallback = std::function<void(IConnection& connection)>;
    using ExceptCallback = std::function<void()>;

public:
    void schedule()
    {
        while (true) {
            update();

            auto res = ::select(max_fd_ + 1, &read_set_, nullptr, &except_set_, nullptr);
            if (res == -1)
                std::cout << strerror(errno);

            dispatch();
        }
    }

    void on_read(ReadCallback read)
    {
        read_ = read;
    }

    void on_except(ExceptCallback except)
    {
        except_ = except;
    }

    // 不要重复注册
    void add_connection(std::unique_ptr<IConnection> connection)
    {
        if (is_registered(*connection))
            return;

        connections_.emplace_back(std::move(connection));
    }

private:
    std::vector<std::unique_ptr<IConnection>> connections_;
    ReadCallback read_;
    ExceptCallback except_;

    fd_set read_set_;
    fd_set except_set_;
    int max_fd_;

    void update()
    {
        std::erase_if(connections_, [] (auto& connection) { return !connection->is_valid(); });
        max_fd_ = -1;

        FD_ZERO(&read_set_);
        FD_ZERO(&except_set_);

        // 注册所有connection
        for (auto& connection: connections_) {
            auto fd = connection->native_handler();
            FD_SET(fd, &read_set_);
            // FIXME: write事件似乎没什么用？？
            FD_SET(fd, &except_set_);
            max_fd_ = std::max(max_fd_, fd);
        }
    }

    void dispatch()
    {
        for (auto& connection: connections_) {
            auto fd = connection->native_handler();

            if (FD_ISSET(fd, &read_set_))
                read_(*connection);

            // FIXME: not a good way
            if (FD_ISSET(fd, &except_set_))
                except_();
        }
    }

    bool is_registered(IConnection& connection)
    {
        auto res = std::find_if(connections_.begin(), connections_.end(),
                [&connection] (auto& value) {
                        return value->native_handler() == connection.native_handler(); });

        return res != connections_.end();
    }
};

// echo
void read_connection(IConnection& connection)
{
    char buffer[4096];
    auto bytes = connection.read(buffer, 4096);
    buffer[bytes] = 0;

    std::cout << "received from client: " << buffer << std::endl;
    if (bytes == 0) {
        connection.close();
        return;
    }

    connection.write(buffer, bytes);
}

void except_connection()
{
    // do nothing
    std::cout << "error happened" << std::endl;
}

int main(int argc, char* argv[])
{
    const Endpoint self_address{ 12345 };

    auto acceptor = make_acceptor();
    acceptor->reuse_address(true);
    acceptor->bind(self_address);
    acceptor->listen();

    Selector selector{};
    selector.on_read(read_connection);
    selector.on_except(except_connection);


    bool once = true;

    while (true) {
        auto connection = acceptor->accept();

        selector.add_connection(std::move(connection));

        if (once) {
            std::thread selector_thread{ &Selector::schedule, std::ref(selector) };
            selector_thread.detach();
            once = false;
        }
    }

    return EXIT_SUCCESS;
}