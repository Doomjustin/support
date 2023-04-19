#include "support/net/Client.h"
#include "support/net/Connector.h"
#include "support/net/Endpoint.h"


namespace support::net {

BasicClient::BasicClient(Domain domain, Type type)
  : acceptor_{ std::make_unique<Connector>(domain, type) }
{}

BasicClient::BasicClient(Endpoint peer, Domain domain, Type type)
  : acceptor_{ std::make_unique<Connector>(domain, type) },
    peer_{ peer }
{}

BasicClient::~BasicClient()
{
    stop();
}

void BasicClient::start()
{
    on_start();
    connection_ = acceptor_->connect(peer_);
    run();
}

void BasicClient::stop()
{
    on_stop();
}

void BasicClient::run()
{
    on_connected(connection_);
}

} // namespace support::net