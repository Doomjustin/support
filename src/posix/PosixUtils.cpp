#include "PosixUtils.h"
#include "support/Log.h"

#include <cerrno>
#include <system_error>
#include <cassert>
#include <netinet/in.h>
#include <arpa/inet.h>


namespace support::posix {

void check_result(int res, std::string_view caller)
{
    if (res == -1) {
        auto cur_errno = errno;
        support::log::error("failed to {}", caller);
        throw std::system_error{ cur_errno,
                                 std::generic_category(),
                                 caller.data() };
    }
}

void check_result(int res, std::source_location cur)
{
    check_result(res, cur.function_name());
}

int enum_cast(::support::net::Domain domain)
{
    switch (domain) {
    using ::support::net::Domain;
    case Domain::IPv4:
        return AF_INET;
    case Domain::IPv6:
        return AF_INET6;
    default:
        assert(false);
        return -1;
    }
}

int enum_cast(::support::net::Type type)
{
    switch (type) {
    using ::support::net::Type;
    case Type::TCP:
        return SOCK_STREAM;
    case Type::UDP:
        return SOCK_DGRAM;
    default:
        assert(false);
        return -1;
    }
}

sockaddr_in cast_to_ipv4_addr(const ::support::net::Endpoint& endpoint)
{
    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_port = htons(endpoint.port());
    if (endpoint.host().empty()) {
        address.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else {
        auto res = ::inet_pton(AF_INET, endpoint.host().data(), &address.sin_addr);
        check_result(res);
    }

    return address;
}

sockaddr_in6 cast_to_ipv6_addr(const ::support::net::Endpoint& endpoint)
{
    sockaddr_in6 server_addr{};
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(endpoint.port());

    if (endpoint.host().empty()) {
        server_addr.sin6_addr = in6addr_any;
    }
    else {
        auto res = ::inet_pton(AF_INET6, endpoint.host().data(), &server_addr.sin6_addr);
        check_result(res);
    }

    return server_addr;
}

::support::net::Endpoint cast_to_endpoint(sockaddr_in& addr)
{
    char buffer[INET_ADDRSTRLEN];
    auto res = ::inet_ntop(AF_INET, &addr.sin_addr, buffer, sizeof(buffer));
    check_result(res != nullptr);

    return ::support::net::Endpoint{ buffer, ntohs(addr.sin_port) };
}

::support::net::Endpoint cast_to_endpoint(sockaddr_in6& addr)
{
    char buffer[INET6_ADDRSTRLEN];
    auto res = ::inet_ntop(AF_INET6, &addr.sin6_addr, buffer, sizeof(buffer));
    check_result(res != nullptr);

    return ::support::net::Endpoint{ buffer, ntohs(addr.sin6_port) };
}

} // namespace support::posix