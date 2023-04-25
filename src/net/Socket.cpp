#include "Socket.h"
#include "../posix/PosixSocket.h"

#include <memory>


namespace support::net {

SocketPtr make_socket(Domain domain, Type type)
{
    return SocketPtr{ new support::posix::PosixSocket{ domain, type }, SocketDeleter{} };
}

} // namespace support::net