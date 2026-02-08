/**
 * @file InputManager.hpp
 * @brief Input handling for keyboard and mouse
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-08
 */

#pragma once

#include "deadcode/core/Types.hpp"

#include <functional>
#include <unordered_map>

namespace deadcode
{

class Window;

/**
 * @brief Input management system
 *
 * Handles keyboard and mouse input via Raylib polling.
 * Provides event-based input handling through callbacks.
 */
class InputManager
{
public:
    // Callback types
    // For Raylib: action = 0 (release), 1 (press), 2 (repeat)
    using KeyCallback         = std::function<void(int key, int scancode, int action, int mods)>;
    using MouseMoveCallback   = std::function<void(double x, double y)>;
    using MouseButtonCallback = std::function<void(int button, int action, int mods)>;

    /**
     * @brief Constructor
     */
    InputManager();

    /**
     * @brief Destructor
     */
    ~InputManager();

    /**
     * @brief Initialize input manager
     *
     * @param window Window to attach input callbacks to
     * @return true if successful
     */
    bool initialize(Window* window);

    /**
     * @brief Shutdown input manager
     */
    void shutdown();

    /**
     * @brief Poll input events and trigger callbacks
     *
     * Must be called each frame to update input state and trigger callbacks.
     */
    void pollEvents();

    /**
     * @brief Set keyboard callback
     *
     * @param callback Function to call on keyboard events
     */
    void setKeyCallback(KeyCallback callback);

    /**
     * @brief Set mouse move callback
     *
     * @param callback Function to call on mouse movement
     */
    void setMouseMoveCallback(MouseMoveCallback callback);

    /**
     * @brief Set mouse button callback
     *
     * @param callback Function to call on mouse button events
     */
    void setMouseButtonCallback(MouseButtonCallback callback);

    /**
     * @brief Get mouse position
     *
     * @param x Output X position
     * @param y Output Y position
     */
    void getMousePosition(double& x, double& y) const;

    // Delete copy and move
    InputManager(const InputManager&)            = delete;
    InputManager& operator=(const InputManager&) = delete;

private:
    Window* m_window{nullptr};
    bool m_initialized{false};

    KeyCallback m_keyCallback;
    MouseMoveCallback m_mouseMoveCallback;
    MouseButtonCallback m_mouseButtonCallback;

    double m_mouseX{0.0};
    double m_mouseY{0.0};

    // Track previous key states for edge detection
    std::unordered_map<int, bool> m_previousKeyStates;
    std::unordered_map<int, bool> m_previousMouseButtonStates;
};

}  // namespace deadcode
