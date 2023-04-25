#include "support/Net.h"
#include "net/Acceptor.h"
#include "net/Connector.h"

#include <memory>


namespace support::net {

std::unique_ptr<IConnector> make_connector(Domain domain, Type type)
{
    return std::make_unique<Connector>(domain, type);
}

std::unique_ptr<IAcceptor> make_acceptor(Domain domain, Type type)
{
    return std::make_unique<Acceptor>(domain, type);
}


SimpleClient::SimpleClient(Domain domain, Type type, const Endpoint& peer)
  : state_{ ClientState::Initializing },
    peer_{ peer },
    connector_{ make_connector(domain, type) },
    connection_{}
{}

void SimpleClient::start()
{
    connection_ = std::move(connector_->connect(peer_));

    begin();
    run();
    end();
}

void SimpleClient::finish() noexcept
{
    state_ = ClientState::Finished;
}

void SimpleClient::run()
{
    state_ = ClientState::Active;

    while (!finished()) {
        ticks(*connection_);
    }
}


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