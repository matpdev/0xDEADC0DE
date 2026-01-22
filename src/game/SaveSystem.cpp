/**
 * @file SaveSystem.cpp
 * @brief Implementation of the SaveSystem class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-22
 */

#include "deadcode/game/SaveSystem.hpp"

#include "deadcode/core/Logger.hpp"

#include <filesystem>

namespace deadcode
{

SaveSystem::SaveSystem() = default;

SaveSystem::~SaveSystem() = default;

bool
SaveSystem::initialize(const String& saveDirectory)
{
    Logger::info("Initializing save system...");

    m_saveDirectory = saveDirectory;

    // Create save directory if it doesn't exist
    try
    {
        if (!std::filesystem::exists(m_saveDirectory))
        {
            std::filesystem::create_directories(m_saveDirectory);
            Logger::info("Created save directory: {}", m_saveDirectory);
        }
    }
    catch (const std::exception& e)
    {
        Logger::error("Failed to create save directory: {}", e.what());
        return false;
    }

    m_initialized = true;
    Logger::info("Save system initialized");
    return true;
}

bool
SaveSystem::hasSaveFiles() const
{
    if (!m_initialized)
        return false;

    try
    {
        if (!std::filesystem::exists(m_saveDirectory))
            return false;

        // Check if directory contains any .sav files
        for (const auto& entry : std::filesystem::directory_iterator(m_saveDirectory))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".sav")
            {
                return true;
            }
        }
    }
    catch (const std::exception& e)
    {
        Logger::error("Error checking for save files: {}", e.what());
    }

    return false;
}

int32
SaveSystem::getSaveFileCount() const
{
    if (!m_initialized)
        return 0;

    int32 count = 0;

    try
    {
        if (!std::filesystem::exists(m_saveDirectory))
            return 0;

        for (const auto& entry : std::filesystem::directory_iterator(m_saveDirectory))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".sav")
            {
                count++;
            }
        }
    }
    catch (const std::exception& e)
    {
        Logger::error("Error counting save files: {}", e.what());
    }

    return count;
}

bool
SaveSystem::saveGame(const String& slotName)
{
    Logger::info("Saving game to slot: {}", slotName);
    // TODO: Implement actual save logic
    Logger::warn("SaveSystem::saveGame() not yet implemented");
    return false;
}

bool
SaveSystem::loadGame(const String& slotName)
{
    Logger::info("Loading game from slot: {}", slotName);
    // TODO: Implement actual load logic
    Logger::warn("SaveSystem::loadGame() not yet implemented");
    return false;
}

}  // namespace deadcode
