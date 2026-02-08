/**
 * @file Renderer.hpp
 * @brief Main rendering coordinator
 *
 * Manages Raylib rendering state and coordinates rendering operations.
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-08
 */

#pragma once

#include "deadcode/core/Types.hpp"
#include "deadcode/graphics/TextRenderer.hpp"

#include <glm/glm.hpp>

#include <memory>

namespace deadcode
{

class Window;

/**
 * @brief Main rendering system coordinator
 *
 * Manages rendering state and provides high-level rendering interface.
 */
class Renderer
{
public:
    /**
     * @brief Constructor
     */
    Renderer();

    /**
     * @brief Destructor
     */
    ~Renderer();

    /**
     * @brief Initialize renderer
     *
     * @param window Window to render to
     * @return true if successful
     */
    bool initialize(Window* window);

    /**
     * @brief Shutdown and cleanup
     */
    void shutdown();

    /**
     * @brief Begin a new frame
     *
     * Clears the screen and prepares for rendering.
     */
    void beginFrame();

    /**
     * @brief End the frame
     *
     * Presents the rendered frame to screen.
     */
    void endFrame();

    /**
     * @brief Set clear color
     *
     * @param color RGB color (each 0-1)
     */
    void setClearColor(const glm::vec3& color);

    /**
     * @brief Get text renderer
     * @return Pointer to text renderer
     */
    [[nodiscard]] TextRenderer* getTextRenderer();

    // Delete copy constructor and assignment
    Renderer(const Renderer&)            = delete;
    Renderer& operator=(const Renderer&) = delete;

private:
    Window* m_window;
    UniquePtr<TextRenderer> m_textRenderer;
    glm::vec3 m_clearColor;
    bool m_initialized;
};

}  // namespace deadcode
