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

} // namespace support::net