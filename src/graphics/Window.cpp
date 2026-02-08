/**
 * @file Window.cpp
 * @brief Implementation of Window class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-08
 */

#include "deadcode/graphics/Window.hpp"

#include "deadcode/core/Logger.hpp"

#include <raylib.h>

#include <stdexcept>

namespace deadcode
{

Window::Window() : m_isOpen(false) {}

Window::~Window()
{
    close();
}

Window::Window(Window&& other) noexcept : m_isOpen(other.m_isOpen), m_config(other.m_config)
{
    other.m_isOpen = false;
}

Window&
Window::operator=(Window&& other) noexcept
{
    if (this != &other)
    {
        close();
        m_isOpen       = other.m_isOpen;
        m_config       = other.m_config;
        other.m_isOpen = false;
    }
    return *this;
}

bool
Window::create(const WindowConfig& config)
{
    m_config = config;

    Logger::info("Creating window: {}x{}", config.width, config.height);

    // Initialize Raylib window
    InitWindow(config.width, config.height, config.title.c_str());

    // Check if window was created successfully
    if (!IsWindowReady())
    {
        Logger::error("Failed to create Raylib window");
        return false;
    }

    m_isOpen = true;

    // Set target FPS
    SetTargetFPS(config.targetFPS);

    // Set fullscreen if requested
    if (config.fullscreen)
    {
        ToggleFullscreen();
        m_config.fullscreen = true;
    }

    // Enable vsync (Raylib handles this through SetTargetFPS and window flags)
    if (!config.vsync)
    {
        SetWindowState(FLAG_VSYNC_HINT);
    }

    Logger::info("Window created successfully");
    Logger::info("Screen dimensions: {}x{}", GetScreenWidth(), GetScreenHeight());

    return true;
}

void
Window::close()
{
    if (m_isOpen)
    {
        Logger::info("Closing window");
        CloseWindow();
        m_isOpen = false;
    }
}

bool
Window::shouldClose() const
{
    return m_isOpen && WindowShouldClose();
}

void
Window::swapBuffers()
{
    // No-op for Raylib - handled internally by BeginDrawing/EndDrawing
    // Kept for API compatibility with old GLFW code
}

int32
Window::getWidth() const
{
    if (!m_isOpen)
        return 0;

    return static_cast<int32>(GetScreenWidth());
}

int32
Window::getHeight() const
{
    if (!m_isOpen)
        return 0;

    return static_cast<int32>(GetScreenHeight());
}

float32
Window::getAspectRatio() const
{
    int32 height = getHeight();
    if (height == 0)
        return 0.0f;
    return static_cast<float32>(getWidth()) / static_cast<float32>(height);
}

bool
Window::isFullscreen() const
{
    return m_config.fullscreen;
}

void
Window::setTitle(const String& title)
{
    m_config.title = title;
    if (m_isOpen)
    {
        SetWindowTitle(title.c_str());
    }
}

}  // namespace deadcode
