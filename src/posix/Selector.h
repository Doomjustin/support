#ifndef CPP_SUPPORT_POSIX_SELECOT_H
#define CPP_SUPPORT_POSIX_SELECOT_H

#include "support/server/Worker.h"

#include <atomic>
#include <condition_variable>
#include <sys/select.h>


namespace support::posix {

class Selector: public support::net::Worker {
public:
    ~Selector() {}

    void start() override;

    void stop() override;

    void add_connection(std::unique_ptr<support::net::IConnection> connection) noexcept override;

private:
    std::atomic_bool is_active_;
    std::condition_variable cv_;
    int max_fd_;
    fd_set read_sets_;
    fd_set except_sets_;

    void update();

    void dispatch();
};

} // namespace support

#endif // CPP_SUPPORT_POSIX_SELECOT_H