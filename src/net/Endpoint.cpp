#include "support/net/Endpoint.h"

#include <sstream>


namespace support::net {

Endpoint::Endpoint(const std::string& host, std::uint16_t port)
  : host_{ host }, port_{ port }
{}

Endpoint::Endpoint(std::uint16_t port)
  : host_{}, port_{ port }
{}

std::string Endpoint::to_string() const noexcept
{
    std::stringstream ss{};

    if (host_.empty())
        ss << "*";
    ss << host_ << ":";

    if (port_ == 0)
        ss << "*";
    else
        ss << port_;

    return ss.str();
}

} // namespace support::net