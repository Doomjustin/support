#include "support/server/SimpleServer.h"


namespace support::net {

SimpleServer::SimpleServer(Domain domain, Type type, const Endpoint& self)
  : state_{ ServerState::Initializing },
    self_{ self },
    acceptor_{ make_acceptor(domain, type) }
{}

void SimpleServer::start()
{
    acceptor_->bind(self_);
    acceptor_->listen(5);

    begin();
    run();
    end();
}

void SimpleServer::finish() noexcept
{
    state_ = ServerState::Finished;
}

void SimpleServer::run()
{
    state_ = ServerState::Active;

    while (!finished()) {
        auto connection = acceptor_->accept();
        on_connect(*connection);
    }
}

} // namespace support::net