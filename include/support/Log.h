#ifndef CPP_SUPPORT_LOG_H
#define CPP_SUPPORT_LOG_H

#include "fmt/core.h"

#include <string_view>
#include <utility>


namespace support {

enum class LogLevel {
    Off,
    Error,
    Warning,
    Debug,
    Info,
    Trace
};

class Logger {
public:
    static Logger* get_logger();

public:
    ~Logger() {}

    virtual void error(const std::string_view message) noexcept = 0;
    virtual void warning(const std::string_view message) noexcept = 0;
    virtual void debug(const std::string_view message) noexcept = 0;
    virtual void info(const std::string_view message) noexcept = 0;
    virtual void trace(const std::string_view message) noexcept = 0;

    constexpr LogLevel level() const noexcept { return level_; }
    virtual void level(LogLevel level) noexcept = 0;

protected:
    LogLevel level_;
};

} // namespace suppoort


namespace support::log {

template<typename... T>
void error(Logger* const logger, fmt::format_string<T...> fmt, T&&... args)
{
    logger->error(fmt::format(fmt, std::forward<T>(args)...));
}

template<typename... T>
void warning(Logger* const logger, fmt::format_string<T...> fmt, T&&... args)
{
    logger->warning(fmt::format(fmt, std::forward<T>(args)...));
}

template<typename... T>
void debug(Logger* const logger, fmt::format_string<T...> fmt, T&&... args)
{
    logger->debug(fmt::format(fmt, std::forward<T>(args)...));
}

template<typename... T>
void info(Logger* const logger, fmt::format_string<T...> fmt, T&&... args)
{
    logger->info(fmt::format(fmt, std::forward<T>(args)...));
}

template<typename... T>
void trace(Logger* const logger, fmt::format_string<T...> fmt, T&&... args)
{
    logger->trace(fmt::format(fmt, std::forward<T>(args)...));
}

// 使用默认logger输出，即Logger::get_logger()返回的logger

template<typename... T>
void error(fmt::format_string<T...> fmt, T&&... args)
{
    Logger::get_logger()->error(fmt::format(fmt, std::forward<T>(args)...));
}

template<typename... T>
void warning(fmt::format_string<T...> fmt, T&&... args)
{
    Logger::get_logger()->warning(fmt::format(fmt, std::forward<T>(args)...));
}

template<typename... T>
void debug(fmt::format_string<T...> fmt, T&&... args)
{
    Logger::get_logger()->debug(fmt::format(fmt, std::forward<T>(args)...));
}

template<typename... T>
void info(fmt::format_string<T...> fmt, T&&... args)
{
    Logger::get_logger()->info(fmt::format(fmt, std::forward<T>(args)...));
}

template<typename... T>
void trace(fmt::format_string<T...> fmt, T&&... args)
{
    Logger::get_logger()->trace(fmt::format(fmt, std::forward<T>(args)...));
}

} // namespace support::log

#endif // CPP_SUPPORT_LOG_H