/**
 * @file Window.hpp
 * @brief Window management with Raylib
 *
 * Wraps Raylib window functionality and manages window creation
 * and event handling.
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-08
 */

#pragma once

#include "deadcode/core/Types.hpp"

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
    int32 targetFPS{60};
};

/**
 * @brief Raylib window wrapper
 *
 * Manages window creation, configuration, and basic window operations.
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
     * @brief Swap front and back buffers (no-op for Raylib, kept for API compatibility)
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

    // Delete copy constructor and assignment
    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;

    // Allow move construction and assignment
    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

private:
    bool m_isOpen;
    WindowConfig m_config;
};

}  // namespace deadcode
