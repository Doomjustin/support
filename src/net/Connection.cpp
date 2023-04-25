#include "support/net/Connection.h"


namespace support::net {

Connection::Connection(std::shared_ptr<Socket> socket)
  : socket_{ socket }
{}

std::size_t Connection::read(void* buffer, std::size_t length)
{
    return socket_->read(buffer, length);
}

std::size_t Connection::write(const void* buffer, std::size_t length)
{
    return socket_->write(buffer, length);
}

void Connection::send(const std::string& value)
{
    socket_->write(value.c_str(), value.size());
}

std::string Connection::receive(std::size_t size)
{
    std::string res{};
    res.resize(size);

    auto real_size = socket_->read(res.data(), res.size());
    res.resize(real_size);

    return res;
}

} // namespace support::net