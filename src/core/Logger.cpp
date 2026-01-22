/**
 * @file Logger.cpp
 * @brief Implementation of the Logger class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-21
 */

#include "deadcode/core/Logger.hpp"

#include <iostream>
#include <vector>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace deadcode
{

std::shared_ptr<spdlog::logger> Logger::s_logger = nullptr;
LogLevel Logger::s_currentLevel                  = LogLevel::INFO;

bool
Logger::initialize(const std::string& logFilePath, LogLevel level)
{
    try
    {
        std::vector<spdlog::sink_ptr> sinks;

        // Console sink with colors
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");
        sinks.push_back(consoleSink);

        // File sink if path is provided
        if (!logFilePath.empty())
        {
            auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);
            fileSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%#] %v");
            sinks.push_back(fileSink);
        }

        // Create logger
        s_logger = std::make_shared<spdlog::logger>("deadcode", sinks.begin(), sinks.end());

        setLevel(level);

        // Register as default logger
        spdlog::set_default_logger(s_logger);

        s_logger->info("Logger initialized successfully");
        return true;
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
        return false;
    }
}

void
Logger::shutdown()
{
    if (s_logger)
    {
        s_logger->info("Logger shutting down");
        s_logger->flush();
        spdlog::shutdown();
        s_logger = nullptr;
    }
}

void
Logger::setLevel(LogLevel level)
{
    s_currentLevel = level;

    if (!s_logger)
    {
        return;
    }

    switch (level)
    {
        case LogLevel::TRACE:
            s_logger->set_level(spdlog::level::trace);
            break;
        case LogLevel::DEBUG:
            s_logger->set_level(spdlog::level::debug);
            break;
        case LogLevel::INFO:
            s_logger->set_level(spdlog::level::info);
            break;
        case LogLevel::WARN:
            s_logger->set_level(spdlog::level::warn);
            break;
        case LogLevel::ERROR:
            s_logger->set_level(spdlog::level::err);
            break;
        case LogLevel::CRITICAL:
            s_logger->set_level(spdlog::level::critical);
            break;
        case LogLevel::OFF:
            s_logger->set_level(spdlog::level::off);
            break;
    }
}

LogLevel
Logger::getLevel()
{
    return s_currentLevel;
}

void
Logger::flush()
{
    if (s_logger)
    {
        s_logger->flush();
    }
}

}  // namespace deadcode
