#ifndef CPP_SUPPORT_NET_ENDPOINT_H
#define CPP_SUPPORT_NET_ENDPOINT_H

#include <cstdint>
#include <string>


namespace support::net {

class Endpoint {
public:
    explicit Endpoint(const std::string& host, std::uint16_t port = 0);
    explicit Endpoint(std::uint16_t port = 0);

    constexpr std::uint16_t port() const noexcept { return port_; }
    std::string_view host() const noexcept { return host_; }

    std::string to_string() const noexcept;

private:
    std::string host_;
    std::uint16_t port_;
};

} // namespace support::net

#endif // CPP_SUPPORT_NET_ENDPOINT_H