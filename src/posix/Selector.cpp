#include "Selector.h"
#include <algorithm>
#include <map>
#include <sys/select.h>


namespace support {

void Selector::schedule()
{
    update();

    auto res = ::select(max_fd_ + 1, &read_fds_, &write_fds_, &except_fds_, nullptr);

    dispatch();
}

void Selector::update()
{
    FD_ZERO(&read_fds_);
    FD_ZERO(&write_fds_);
    FD_ZERO(&except_fds_);
    max_fd_ = -1;

    // 移除所有失效的event
    std::erase_if(events_, [] (auto pair) { return !pair.second->is_valid(); });

    for (auto& [fd, event]: events_) {
        max_fd_ = std::max(fd, max_fd_);

        if (event->readable())
            FD_SET(fd, &read_fds_);

        if (event->writable())
            FD_SET(fd, &write_fds_);

        if (event->exceptable())
            FD_SET(fd, &except_fds_);
    }
}

void Selector::dispatch()
{
    for (auto& [fd, event]: events_) {
        if (FD_ISSET(fd, &read_fds_))
            event->read();

        if (FD_ISSET(fd, &write_fds_))
            event->write();

        if (FD_ISSET(fd, &except_fds_))
            event->except();
    }
}

} // namespace support