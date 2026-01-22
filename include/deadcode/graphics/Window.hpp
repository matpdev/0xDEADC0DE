/**
 * @file Window.hpp
 * @brief Window management with OpenGL context
 *
 * Wraps GLFW window functionality and manages OpenGL context creation
 * and event handling.
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-21
 */

#pragma once

#include "deadcode/core/Types.hpp"

#include <functional>
#include <string>

struct GLFWwindow;

namespace deadcode
{

/**
 * @brief Window configuration parameters
 */
struct WindowConfig
{
    String title{"0xDEADC0DE RPG"};
    int32 width{800};
    int32 height{600};
    bool fullscreen{false};
    bool vsync{true};
    int32 glMajorVersion{3};
    int32 glMinorVersion{3};
};

/**
 * @brief GLFW window wrapper with OpenGL context
 *
 * Manages window creation, OpenGL context initialization,
 * and basic window operations.
 */
class Window
{
public:
    /**
     * @brief Construct window (does not create it)
     */
    Window();

    /**
     * @brief Destructor, closes window if open
     */
    ~Window();

    /**
     * @brief Create and initialize the window
     *
     * @param config Window configuration
     * @return true if window creation succeeded
     */
    bool create(const WindowConfig& config);

    /**
     * @brief Close and destroy the window
     */
    void close();

    /**
     * @brief Check if window should close
     * @return true if window close was requested
     */
    [[nodiscard]] bool shouldClose() const;

    /**
     * @brief Swap front and back buffers
     */
    void swapBuffers();

    /**
     * @brief Get window width
     * @return Current window width in pixels
     */
    [[nodiscard]] int32 getWidth() const;

    /**
     * @brief Get window height
     * @return Current window height in pixels
     */
    [[nodiscard]] int32 getHeight() const;

    /**
     * @brief Get aspect ratio
     * @return Width divided by height
     */
    [[nodiscard]] float32 getAspectRatio() const;

    /**
     * @brief Check if window is in fullscreen mode
     * @return true if fullscreen
     */
    [[nodiscard]] bool isFullscreen() const;

    /**
     * @brief Set window title
     * @param title New window title
     */
    void setTitle(const String& title);

    /**
     * @brief Get native GLFW window handle
     * @return Pointer to GLFWwindow
     */
    [[nodiscard]] GLFWwindow* getNativeWindow() const;

    // Delete copy constructor and assignment
    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;

    // Allow move construction and assignment
    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

private:
    /**
     * @brief Setup OpenGL context and GLEW
     * @return true if successful
     */
    bool setupOpenGL();

    /**
     * @brief Framebuffer size callback
     */
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* m_window;
    WindowConfig m_config;
};

}  // namespace deadcode
