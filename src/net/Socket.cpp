#include "support/net/Socket.h"
#include "../posix/net/PosixSocket.h"

#include <memory>


namespace support::net {

Socket::Socket(Domain domain, Type type)
  : socket_{ -1 },
    domain_{ domain },
    type_{ type }
{}

std::shared_ptr<Socket> open(Domain domain, Type type)
{
    return std::make_shared<support::net::posix::PosixSocket>(domain, type);
}

} // namespace support::net