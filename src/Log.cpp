#include "support/Log.h"
#include "log/SpdLogger.h"


namespace support {

Logger* Logger::get_logger()
{
    static SpdLogger logger{};

    return &logger;
}

} // namespace support