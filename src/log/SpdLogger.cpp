#include "SpdLogger.h"

#include "spdlog/common.h"
#include "spdlog/sinks/stdout_color_sinks.h"


namespace support {

SpdLogger::SpdLogger()
  : console_logger_{ spdlog::stdout_color_mt("console") }
{}

void SpdLogger::error(const std::string_view message) noexcept
{
    console_logger_->error(message);
}

void SpdLogger::warning(const std::string_view message) noexcept
{
    console_logger_->warn(message);
}

void SpdLogger::debug(const std::string_view message) noexcept
{
    console_logger_->debug(message);
}

void SpdLogger::info(const std::string_view message) noexcept
{
    console_logger_->info(message);
}

void SpdLogger::trace(const std::string_view message) noexcept
{
    console_logger_->trace(message);
}

void SpdLogger::level(LogLevel level) noexcept
{
    switch (level) {
    case LogLevel::Off:
        console_logger_->set_level(spdlog::level::off);
        break;
    case LogLevel::Error:
        console_logger_->set_level(spdlog::level::err);
        break;
    case LogLevel::Warning:
        console_logger_->set_level(spdlog::level::warn);
        break;
    case LogLevel::Debug:
        console_logger_->set_level(spdlog::level::debug);
        break;
    case LogLevel::Info:
        console_logger_->set_level(spdlog::level::info);
        break;
    case LogLevel::Trace:
        console_logger_->set_level(spdlog::level::trace);
        break;
    }
}

} // namespace support