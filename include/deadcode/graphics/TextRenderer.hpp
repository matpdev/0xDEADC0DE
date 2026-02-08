/**
 * @file TextRenderer.hpp
 * @brief Text rendering with Raylib fonts
 *
 * Manages font loading and text rendering using Raylib.
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-08
 */

#pragma once

#include "deadcode/core/Types.hpp"

#include <glm/glm.hpp>
#include <raylib.h>

#include <functional>
#include <string>

namespace deadcode
{

/**
 * @brief Text rendering system using Raylib
 *
 * Manages font loading and rendering text to screen using
 * Raylib's font API.
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
    Font m_font;
    float32 m_fontSize;
    int32 m_screenWidth;
    int32 m_screenHeight;
    bool m_initialized;
    bool m_fontLoaded;
};

}  // namespace deadcode
