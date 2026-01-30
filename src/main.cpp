/**
 * @file main.cpp
 * @brief Application entry point
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-21
 */

#include "deadcode/core/Application.hpp"
#include "deadcode/core/Logger.hpp"
#include "deadcode/core/Version.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>

/**
 * @brief Main entry point for the 0xDEADC0DE text-based RPG
 *
 * @param argc Command line argument count
 * @param argv Command line arguments
 * @return EXIT_SUCCESS on successful execution, EXIT_FAILURE otherwise
 */
int
main(int argc, char** argv)
{
    try
    {
        // Initialize logging first
        if (!deadcode::Logger::initialize("deadcode.log", deadcode::LogLevel::DEBUG))
        {
            std::cerr << "Failed to initialize logging system\n";
            return EXIT_FAILURE;
        }

        deadcode::Logger::info("========================================");
        deadcode::Logger::info("{} - Text-Based RPG", deadcode::Version::getGameTitleWithVersion());
        deadcode::Logger::info("Build: {}", deadcode::Version::BUILD_TYPE);
        deadcode::Logger::info("========================================");

        // Get application instance and initialize
        auto& app = deadcode::Application::getInstance();

        if (!app.initialize(argc, argv))
        {
            deadcode::Logger::critical("Failed to initialize application");
            deadcode::Logger::shutdown();
            return EXIT_FAILURE;
        }

        deadcode::Logger::info("Application initialized successfully");

        // Run the main game loop
        app.run();

        deadcode::Logger::info("Application shutting down...");

        // Cleanup
        app.shutdown();

        deadcode::Logger::info("Shutdown complete");
        deadcode::Logger::info("========================================");
        deadcode::Logger::shutdown();

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        deadcode::Logger::critical("Fatal exception: {}", e.what());
        deadcode::Logger::shutdown();
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Unknown fatal error occurred\n";
        deadcode::Logger::critical("Unknown fatal exception occurred");
        deadcode::Logger::shutdown();
        return EXIT_FAILURE;
    }
}
