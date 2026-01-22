/**
 * @file Application.hpp
 * @brief Main application class managing the engine lifecycle
 *
 * The Application class is the central coordinator for the entire engine,
 * managing initialization, the main game loop, and shutdown of all subsystems.
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-21
 */

#pragma once

#include "deadcode/core/Types.hpp"
#include "deadcode/game/GameState.hpp"

#include <memory>
#include <string>

namespace deadcode
{

class Menu;
class InputManager;
class SaveSystem;

/**
 * @brief Main application class following the Singleton pattern
 *
 * Manages the complete lifecycle of the game engine including:
 * - Subsystem initialization and shutdown
 * - Main game loop coordination
 * - Event dispatching
 * - Frame timing
 */
class Application
{
public:
    /**
     * @brief Get the singleton instance
     * @return Reference to the Application instance
     */
    static Application& getInstance();

    /**
     * @brief Initialize the application and all subsystems
     *
     * Initializes:
     * - Logger
     * - Configuration
     * - Window and rendering context
     * - Input system
     * - Audio system
     * - Resource manager
     *
     * @param argc Command line argument count
     * @param argv Command line arguments
     * @return true if initialization succeeded, false otherwise
     */
    bool initialize(int argc, char** argv);

    /**
     * @brief Run the main game loop
     *
     * Executes the game loop until exit is requested:
     * 1. Process input events
     * 2. Update game state
     * 3. Render frame
     * 4. Synchronize timing
     */
    void run();

    /**
     * @brief Shutdown the application and cleanup resources
     *
     * Performs orderly shutdown of all subsystems in reverse
     * initialization order.
     */
    void shutdown();

    /**
     * @brief Request application exit
     *
     * Sets a flag to exit the main loop gracefully at the end
     * of the current frame.
     */
    void requestExit();

    /**
     * @brief Check if exit has been requested
     * @return true if application should exit
     */
    [[nodiscard]] bool isExitRequested() const;

    /**
     * @brief Get the target frames per second
     * @return Target FPS
     */
    [[nodiscard]] int getTargetFPS() const;

    /**
     * @brief Set the target frames per second
     * @param fps Target FPS (0 for unlimited)
     */
    void setTargetFPS(int fps);

    /**
     * @brief Get the current frames per second
     * @return Current FPS
     */
    [[nodiscard]] float getCurrentFPS() const;

    // Delete copy and move constructors/assignment operators
    Application(const Application&)            = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&)                 = delete;
    Application& operator=(Application&&)      = delete;

private:
    /**
     * @brief Private constructor for singleton pattern
     */
    Application();

    /**
     * @brief Destructor
     */
    ~Application();

    /**
     * @brief Initialize logging system
     * @return true if successful
     */
    bool initializeLogger();

    /**
     * @brief Initialize configuration system
     * @return true if successful
     */
    bool initializeConfig();

    /**
     * @brief Initialize window and graphics context
     * @return true if successful
     */
    bool initializeWindow();

    /**
     * @brief Initialize rendering system
     * @return true if successful
     */
    bool initializeRenderer();

    /**
     * @brief Initialize input system
     * @return true if successful
     */
    bool initializeInput();

    /**
     * @brief Initialize audio system
     * @return true if successful
     */
    bool initializeAudio();

    /**
     * @brief Initialize resource manager
     * @return true if successful
     */
    bool initializeResources();

    /**
     * @brief Initialize game systems
     * @return true if successful
     */
    bool initializeGame();

    /**
     * @brief Process input events
     * @param deltaTime Time since last frame in seconds
     */
    void processInput(float deltaTime);

    /**
     * @brief Update game state
     * @param deltaTime Time since last frame in seconds
     */
    void update(float deltaTime);

    /**
     * @brief Render the current frame
     * @param deltaTime Time since last frame in seconds
     */
    void render(float deltaTime);

    /**
     * @brief Calculate and enforce frame timing
     */
    void syncFrameRate();

    /**
     * @brief Setup main menu items
     */
    void setupMainMenu();

    /**
     * @brief Handle keyboard input
     */
    void handleKeyInput(int key, int scancode, int action, int mods);

    /**
     * @brief Handle mouse movement
     */
    void handleMouseMove(double x, double y);

    /**
     * @brief Handle mouse button input
     */
    void handleMouseButton(int button, int action, int mods);

    /**
     * @brief Handle window resize events
     */
    void handleWindowResize();

    // Subsystems (using Pimpl idiom for better compilation times)
    struct Impl;
    UniquePtr<Impl> m_impl;

    // Application state
    bool m_initialized;
    bool m_running;
    bool m_exitRequested;
    int m_targetFPS;
    float m_currentFPS;
    GameState m_gameState{GameState::MainMenu};
};

}  // namespace deadcode
