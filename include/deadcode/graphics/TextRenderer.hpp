/**
 * @file TextRenderer.hpp
 * @brief Text rendering with FreeType fonts
 *
 * Manages font loading and text rendering using OpenGL and FreeType.
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-21
 */

#pragma once

#include "deadcode/core/Types.hpp"
#include "deadcode/graphics/Shader.hpp"

#include <glm/glm.hpp>

#include <map>
#include <memory>
#include <string>

namespace deadcode
{

/**
 * @brief Character glyph information
 */
struct Character
{
    uint32 textureID;    ///< OpenGL texture ID
    glm::ivec2 size;     ///< Size of glyph
    glm::ivec2 bearing;  ///< Offset from baseline to left/top of glyph
    uint32 advance;      ///< Horizontal offset to next glyph
};

/**
 * @brief Text rendering system using FreeType
 *
 * Manages font loading and rendering text to screen using
 * OpenGL and FreeType library.
 */
class TextRenderer
{
public:
    /**
     * @brief Constructor
     */
    TextRenderer();

    /**
     * @brief Destructor
     */
    ~TextRenderer();

    /**
     * @brief Initialize text rendering system
     *
     * @param screenWidth Screen width in pixels
     * @param screenHeight Screen height in pixels
     * @return true if successful
     */
    bool initialize(int32 screenWidth, int32 screenHeight);

    /**
     * @brief Shutdown and cleanup resources
     */
    void shutdown();

    /**
     * @brief Load a font from file
     *
     * @param fontPath Path to TTF font file
     * @param fontSize Font size in pixels
     * @return true if successful
     */
    bool loadFont(const String& fontPath, uint32 fontSize);

    /**
     * @brief Render text to screen
     *
     * @param text Text string to render
     * @param x X position in screen coordinates
     * @param y Y position in screen coordinates
     * @param scale Text scale factor
     * @param color Text color (RGB, each 0-1)
     */
    void renderText(const String& text, float32 x, float32 y, float32 scale,
                    const glm::vec3& color);

    /**
     * @brief Render text with per-character transformation callback
     *
     * Allows applying per-character effects like glitch displacement
     *
     * @param text Text string to render
     * @param x X position in screen coordinates
     * @param y Y position in screen coordinates
     * @param scale Text scale factor
     * @param color Base text color (RGB, each 0-1)
     * @param charCallback Function called for each character with (charIndex, characterCount,
     * baseX, baseY) returning (offsetX, offsetY, colorMod, visible)
     */
    void renderTextWithCallback(const String& text, float32 x, float32 y, float32 scale,
                                const glm::vec3& color,
                                std::function<void(uint32 charIndex, uint32 charCount, float32& x,
                                                   float32& y, glm::vec3& color, bool& visible)>
                                    charCallback);

    /**
     * @brief Update screen dimensions (for window resize)
     *
     * @param width New screen width
     * @param height New screen height
     */
    void updateScreenSize(int32 width, int32 height);

    /**
     * @brief Calculate the width of a text string
     *
     * @param text Text string to measure
     * @param scale Text scale factor
     * @return Width in pixels
     */
    float32 getTextWidth(const String& text, float32 scale) const;

    float32 getCharWidth(float32 scale) const;

    float32 getLineHeight(float32 scale) const;

    // Delete copy constructor and assignment
    TextRenderer(const TextRenderer&)            = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;

private:
    /**
     * @brief Setup OpenGL buffers and vertex arrays
     */
    void setupRenderData();

    std::map<char, Character> m_characters;
    Shader m_shader;
    uint32 m_VAO;
    uint32 m_VBO;
    glm::mat4 m_projection;
    bool m_initialized;
};

}  // namespace deadcode
