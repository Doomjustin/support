#include "support/Server/Worker.h"

#include "../posix/Selector.h"
#include <memory>


namespace support::net {

std::size_t Worker::size() const noexcept
{
    std::lock_guard<std::mutex> locker{ m_ };
    return connections_.size();
}

void Worker::add_connection(std::unique_ptr<IConnection> connection) noexcept
{
    std::lock_guard<std::mutex> locker{ m_ };
    if (is_registered(*connection))
        return;
    connections_.emplace_back(std::move(connection));
}

void Worker::erase_invalid_connections()
{
    std::erase_if(connections_, [] (auto& connection) { return !connection->is_valid(); });
}

bool Worker::is_registered(IConnection& connection) const
{
    auto res = std::find_if(connections_.begin(), connections_.end(),
            [&connection] (auto& value) {
                    return value->native_handler() == connection.native_handler(); });

    return res != connections_.end();
}

std::unique_ptr<Worker> make_worker()
{
    return std::make_unique<posix::Selector>();
}

} // namespace support::net