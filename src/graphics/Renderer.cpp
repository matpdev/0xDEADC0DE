/**
 * @file Renderer.cpp
 * @brief Implementation of Renderer class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-08
 */

#include "deadcode/graphics/Renderer.hpp"

#include "deadcode/core/Logger.hpp"
#include "deadcode/graphics/Window.hpp"

#include <raylib.h>

namespace deadcode
{

Renderer::Renderer() : m_window(nullptr), m_clearColor(0.0f, 0.0f, 0.0f), m_initialized(false) {}

Renderer::~Renderer()
{
    shutdown();
}

bool
Renderer::initialize(Window* window)
{
    if (!window)
    {
        Logger::error("Cannot initialize renderer with null window");
        return false;
    }

    m_window = window;

    Logger::info("Initializing Renderer...");

    // Initialize text renderer
    m_textRenderer = std::make_unique<TextRenderer>();
    if (!m_textRenderer->initialize(window->getWidth(), window->getHeight()))
    {
        Logger::error("Failed to initialize TextRenderer");
        return false;
    }

    m_initialized = true;
    Logger::info("Renderer initialized successfully");
    return true;
}

void
Renderer::shutdown()
{
    if (!m_initialized)
        return;

    Logger::info("Shutting down Renderer...");

    if (m_textRenderer)
    {
        m_textRenderer->shutdown();
        m_textRenderer.reset();
    }

    m_initialized = false;
}

void
Renderer::beginFrame()
{
    // Begin Raylib drawing
    BeginDrawing();

    // Clear screen with background color
    Color clearColor = {static_cast<uint8>(m_clearColor.r * 255.0f),
                        static_cast<uint8>(m_clearColor.g * 255.0f),
                        static_cast<uint8>(m_clearColor.b * 255.0f), 255};
    ClearBackground(clearColor);
}

void
Renderer::endFrame()
{
    // End Raylib drawing
    EndDrawing();
}

void
Renderer::setClearColor(const glm::vec3& color)
{
    m_clearColor = color;
}

TextRenderer*
Renderer::getTextRenderer()
{
    return m_textRenderer.get();
}

}  // namespace deadcode
