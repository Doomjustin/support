#include "support/server/MultiServer.h"
#include "support/Net.h"

#include <future>
#include <thread>


namespace support::net {

std::uint8_t MultiThreadServer::max_threads_ = 16;  // 16


MultiThreadServer::MultiThreadServer(const Endpoint& self, Domain domain, Type type)
  : self_{ self }, acceptor_{ make_acceptor(domain, type) }
{}

void MultiThreadServer::start()
{
    acceptor_->bind(self_);
    acceptor_->listen();
    acceptor_->reuse_address();

    while (true) {
        auto new_connection = acceptor_->accept();

        // 放到一个合适的worker中执行 -> 空闲或者连接数最少的worker
        auto available_manager = get_available_worker();
        available_manager->add_connection(std::move(new_connection));
    }
}

void MultiThreadServer::create_new_worker()
{
    auto connection_manager = make_worker();
    using namespace std::placeholders;
    connection_manager->on_read(std::bind(&MultiThreadServer::on_read, this, _1));
    // TODO: write事件有意义吗？
    connection_manager->on_except(std::bind(&MultiThreadServer::on_except, this, _1));
    connection_managers_.emplace_back(std::move(connection_manager));

    // 不管理thread
    std::thread t{ &Worker::start, std::ref(*connection_managers_.back()) };
    t.detach();
}

// 适合的worker来放置新的connection -> 空闲或者连接数最少的worker
Worker* MultiThreadServer::get_available_worker()
{
    // 查找空闲worker或者创建新worker
    if (connection_managers_.size() < max_threads_) {
        // 查找是否有空闲的worker
        for (auto& manager: connection_managers_)
            if (manager->size() == 0)
                return manager.get();
        // 否则创建新的worker
        create_new_worker();
        return connection_managers_.back().get();
    }

    // 查找connections最少得worker
    int least_size = 0;
    int least_index = 0;
    for (int i = 0; i < connection_managers_.size(); ++i) {
        if (connection_managers_[i]->size() < least_size) {
            least_index = i;
            least_size = connection_managers_[i]->size();
        }
    }

    return connection_managers_[least_index].get();
}

} // namespace support::net