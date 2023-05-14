#include "support/client/SimpleClient.h"


namespace support::net {

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

} // namespace support::net