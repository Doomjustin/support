#ifndef CPP_SUPPORT_SERVER_WORKER_H
#define CPP_SUPPORT_SERVER_WORKER_H

#include "../Net.h"

#include <cstddef>
#include <functional>
#include <mutex>
#include <vector>
#include <memory>


namespace support::net {

class Worker {
public:
    using ReadCallback = std::function<void(IConnection&)>;
    using ExceptCallback = std::function<void(IConnection&)>;

public:
    ~Worker() {}

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual void add_connection(std::unique_ptr<IConnection> connection) noexcept;

    std::size_t size() const noexcept;

    void on_read(ReadCallback read) noexcept
    {
        read_ = read;
    }

    void on_except(ExceptCallback except) noexcept
    {
        except_ = except;
    }

protected:
    mutable std::mutex m_;
    std::vector<std::unique_ptr<IConnection>> connections_;
    ReadCallback read_;
    ExceptCallback except_;

    void erase_invalid_connections();
    bool is_registered(IConnection& connection) const;
};

std::unique_ptr<Worker> make_worker();

} // namespace support::net

#endif // CPP_SUPPORT_SERVER_WORKER_H