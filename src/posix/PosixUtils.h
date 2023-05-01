#ifndef CPP_SUPPORT_POSIX_POSIX_UTILS_H
#define CPP_SUPPORT_POSIX_POSIX_UTILS_H

#include "support/Net.h"

#include <string>
#include <source_location>
#include <netinet/in.h>


namespace support::posix {

void check_result(int res, const std::string& caller);
void check_result(int res, std::source_location cur = std::source_location::current());

int enum_cast(::support::net::Domain domain);
int enum_cast(::support::net::Type type);

sockaddr_in cast_to_ipv4_addr(const ::support::net::Endpoint& endpoint);
sockaddr_in6 cast_to_ipv6_addr(const ::support::net::Endpoint& endpoint);

::support::net::Endpoint cast_to_endpoint(sockaddr_in& addr);
::support::net::Endpoint cast_to_endpoint(sockaddr_in6& addr);

} // namespace support

#endif // CPP_SUPPORT_POSIX_POSIX_UTILS_H