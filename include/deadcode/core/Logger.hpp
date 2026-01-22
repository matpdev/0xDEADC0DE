/**
 * @file Logger.hpp
 * @brief Centralized logging system wrapper around spdlog
 *
 * Provides a convenient interface for logging throughout the engine
 * with multiple severity levels and output targets.
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-21
 */

#pragma once

#include <memory>
#include <string>

#include <spdlog/spdlog.h>

namespace deadcode
{

/**
 * @brief Logging severity levels
 */
enum class LogLevel
{
    TRACE,     ///< Detailed trace information
    DEBUG,     ///< Debug information
    INFO,      ///< General information
    WARN,      ///< Warning messages
    ERROR,     ///< Error messages
    CRITICAL,  ///< Critical errors
    OFF        ///< Disable logging
};

/**
 * @brief Centralized logging system
 *
 * Thread-safe logging system with support for:
 * - Multiple log levels
 * - Console and file output
 * - Colored console output
 * - Custom formatting
 * - Pattern-based filtering
 */
class Logger
{
public:
    /**
     * @brief Initialize the logging system
     *
     * Sets up console and file loggers with appropriate formatting.
     *
     * @param logFilePath Path to log file (empty for console only)
     * @param level Minimum log level to output
     * @return true if initialization succeeded
     */
    static bool initialize(const std::string& logFilePath = "deadcode.log",
                           LogLevel level                 = LogLevel::INFO);

    /**
     * @brief Shutdown the logging system
     *
     * Flushes all pending log messages and closes file handles.
     */
    static void shutdown();

    /**
     * @brief Set the minimum log level
     * @param level Minimum level to output
     */
    static void setLevel(LogLevel level);

    /**
     * @brief Get the current log level
     * @return Current log level
     */
    [[nodiscard]] static LogLevel getLevel();

    /**
     * @brief Flush all pending log messages
     */
    static void flush();

    /**
     * @brief Log a trace message
     *
     * @tparam Args Variadic template for format arguments
     * @param fmt Format string
     * @param args Format arguments
     */
    template <typename... Args>
    static void
    trace(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (s_logger)
        {
            s_logger->trace(fmt, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Log a debug message
     *
     * @tparam Args Variadic template for format arguments
     * @param fmt Format string
     * @param args Format arguments
     */
    template <typename... Args>
    static void
    debug(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (s_logger)
        {
            s_logger->debug(fmt, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Log an info message
     *
     * @tparam Args Variadic template for format arguments
     * @param fmt Format string
     * @param args Format arguments
     */
    template <typename... Args>
    static void
    info(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (s_logger)
        {
            s_logger->info(fmt, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Log a warning message
     *
     * @tparam Args Variadic template for format arguments
     * @param fmt Format string
     * @param args Format arguments
     */
    template <typename... Args>
    static void
    warn(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (s_logger)
        {
            s_logger->warn(fmt, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Log an error message
     *
     * @tparam Args Variadic template for format arguments
     * @param fmt Format string
     * @param args Format arguments
     */
    template <typename... Args>
    static void
    error(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (s_logger)
        {
            s_logger->error(fmt, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Log a critical message
     *
     * @tparam Args Variadic template for format arguments
     * @param fmt Format string
     * @param args Format arguments
     */
    template <typename... Args>
    static void
    critical(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (s_logger)
        {
            s_logger->critical(fmt, std::forward<Args>(args)...);
        }
    }

private:
    static std::shared_ptr<spdlog::logger> s_logger;
    static LogLevel s_currentLevel;
};

}  // namespace deadcode
