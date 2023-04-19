#include "support/Log.h"

#include <cstdlib>


int main(int argc, char* argv[])
{
    using namespace support::log;

    auto logger = support::Logger::get_logger();

    error(logger, "{}: have a nice try, with fmt number {}", "error", 1);
    warning(logger, "{}: have a nice try, with fmt number {}", "warning", 1);
    debug(logger, "{}: have a nice try, with fmt number {}", "debug", 1);
    info(logger, "{}: have a nice try, with fmt number {}", "info", 1);
    trace(logger, "{}: have a nice try, with fmt number {}", "trace", 1);

    error("{}: have a nice try, with fmt number {}", "error", 1);
    warning("{}: have a nice try, with fmt number {}", "warning", 1);
    debug("{}: have a nice try, with fmt number {}", "debug", 1);
    info("{}: have a nice try, with fmt number {}", "info", 1);
    trace("{}: have a nice try, with fmt number {}", "trace", 1);

    return EXIT_SUCCESS;
}