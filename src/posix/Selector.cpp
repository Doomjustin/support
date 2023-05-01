#include "Selector.h"
#include "PosixUtils.h"

#include <algorithm>
#include <memory>
#include <mutex>
#include <sys/select.h>


namespace support::posix {

void Selector::start()
{
    is_active_ = true;

    while (true) {
        std::unique_lock<std::mutex> locker{ m_ };
        cv_.wait(locker, [this] () { return !connections_.empty(); });
        locker.unlock();

        update();

        auto res = ::select(max_fd_ + 1, &read_sets_, nullptr, &except_sets_, nullptr);
        check_result(res);

        dispatch();
    }
}

void Selector::stop()
{
    is_active_ = false;
}

void Selector::add_connection(std::unique_ptr<support::net::IConnection> connection) noexcept
{
    support::net::ConnectionManager::add_connection(std::move(connection));
    cv_.notify_one();
}

void Selector::update()
{
    FD_ZERO(&read_sets_);
    FD_ZERO(&except_sets_);
    max_fd_ = -1;

    std::lock_guard<std::mutex> locker{ m_ };
    for (auto& connection: connections_) {
        auto fd = connection->native_handler();
        FD_SET(fd, &read_sets_);
        FD_SET(fd, &except_sets_);
        max_fd_ = std::max(max_fd_, fd);
    }
}

void Selector::dispatch()
{
    std::lock_guard<std::mutex> locker{ m_ };
    for (auto& connection: connections_) {
        auto fd = connection->native_handler();

        if (FD_ISSET(fd, &read_sets_))
            read_(*connection);

        if (FD_ISSET(fd, &except_sets_))
            except_(*connection);
    }
}

} // namespace support::posix