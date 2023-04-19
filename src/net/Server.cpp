#include "support/net/Server.h"
#include "support/net/Acceptor.h"

#include <memory>


namespace support::net {

BasicServer::BasicServer(Domain domain, Type type)
  : acceptor_{ std::make_unique<Acceptor>(domain, type) },
    state_{ ServerState::Init }
{}

BasicServer::~BasicServer()
{
    stop();
}

void BasicServer::start()
{
    acceptor_->bind(self_);
    acceptor_->listen(listen_backlog_);

    on_start();
    state_ = ServerState::Pending;
    run();
}

void BasicServer::stop()
{
    state_ = ServerState::Exit;
    on_stop();
}

void BasicServer::run()
{
    while (!should_stop()) {
        auto connection = acceptor_->accept();

        on_connected(connection);
    }
}

} // namespace support::net