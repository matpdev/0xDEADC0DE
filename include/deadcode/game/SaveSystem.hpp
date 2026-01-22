/**
 * @file SaveSystem.hpp
 * @brief Game save/load management
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-22
 */

#pragma once

#include "deadcode/core/Types.hpp"

#include <string>

namespace deadcode
{

/**
 * @brief Save file management system
 *
 * Handles saving and loading game state to/from disk.
 */
class SaveSystem
{
public:
    /**
     * @brief Constructor
     */
    SaveSystem();

    /**
     * @brief Destructor
     */
    ~SaveSystem();

    /**
     * @brief Initialize save system
     *
     * @param saveDirectory Directory to store save files
     * @return true if successful
     */
    bool initialize(const String& saveDirectory = "saves");

    /**
     * @brief Check if any save files exist
     *
     * @return true if at least one save file exists
     */
    [[nodiscard]] bool hasSaveFiles() const;

    /**
     * @brief Get number of save files
     *
     * @return Number of save files found
     */
    [[nodiscard]] int32 getSaveFileCount() const;

    /**
     * @brief Save game data
     *
     * @param slotName Name of save slot
     * @return true if save succeeded
     */
    bool saveGame(const String& slotName);

    /**
     * @brief Load game data
     *
     * @param slotName Name of save slot
     * @return true if load succeeded
     */
    bool loadGame(const String& slotName);

    // Delete copy and move
    SaveSystem(const SaveSystem&)            = delete;
    SaveSystem& operator=(const SaveSystem&) = delete;

private:
    String m_saveDirectory;
    bool m_initialized{false};
};

}  // namespace deadcode
