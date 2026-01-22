/**
 * @file GameState.hpp
 * @brief Game state enumeration and management
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-22
 */

#pragma once

namespace deadcode
{

/**
 * @brief Game state enumeration
 *
 * Defines the different states the game can be in.
 */
enum class GameState
{
    MainMenu,       ///< Main menu screen
    Playing,        ///< Active gameplay
    Paused,         ///< Game paused
    Configuration,  ///< Configuration/settings screen
    Loading,        ///< Loading screen
    Exiting         ///< Shutting down
};

}  // namespace deadcode
