#ifndef CPP_SUPPORT_LOG_SPDLOGGER_H
#define CPP_SUPPORT_LOG_SPDLOGGER_H

#include "support/Log.h"

#include "spdlog/spdlog.h"

#include <memory>


namespace support {

class SpdLogger: public Logger {
public:
    SpdLogger();

    ~SpdLogger() {}

    void error(const std::string_view message) noexcept override;
    void warning(const std::string_view message) noexcept override;
    void debug(const std::string_view message) noexcept override;
    void info(const std::string_view message) noexcept override;
    void trace(const std::string_view message) noexcept override;

    void level(LogLevel level) noexcept override;

private:
    std::shared_ptr<spdlog::logger> console_logger_;
};

} // namespace support

#endif // CPP_SUPPORT_LOG_SPDLOGGER_H