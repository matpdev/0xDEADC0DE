/**
 * @file InputManager.hpp
 * @brief Input handling for keyboard and mouse
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-22
 */

#pragma once

#include "deadcode/core/Types.hpp"

#include <functional>

struct GLFWwindow;

namespace deadcode
{

class Window;

/**
 * @brief Input management system
 *
 * Handles keyboard and mouse input via GLFW callbacks.
 * Provides event-based input handling.
 */
class InputManager
{
public:
    // Callback types
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
    /**
     * @brief GLFW key callback
     */
    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    /**
     * @brief GLFW cursor position callback
     */
    static void glfwCursorPosCallback(GLFWwindow* window, double x, double y);

    /**
     * @brief GLFW mouse button callback
     */
    static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    GLFWwindow* m_window{nullptr};
    bool m_initialized{false};

    KeyCallback m_keyCallback;
    MouseMoveCallback m_mouseMoveCallback;
    MouseButtonCallback m_mouseButtonCallback;

    double m_mouseX{0.0};
    double m_mouseY{0.0};
};

}  // namespace deadcode
