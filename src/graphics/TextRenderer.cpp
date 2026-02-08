/**
 * @file TextRenderer.cpp
 * @brief Implementation of TextRenderer class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-08
 */

#include "deadcode/graphics/TextRenderer.hpp"

#include "deadcode/core/Logger.hpp"

#include <raylib.h>

namespace deadcode
{

TextRenderer::TextRenderer()
    : m_fontSize(0.0f), m_screenWidth(0), m_screenHeight(0), m_initialized(false), m_fontLoaded(false)
{
}

TextRenderer::~TextRenderer()
{
    shutdown();
}

bool
TextRenderer::initialize(int32 screenWidth, int32 screenHeight)
{
    Logger::info("Initializing TextRenderer...");

    m_screenWidth  = screenWidth;
    m_screenHeight = screenHeight;

    m_initialized = true;
    Logger::info("TextRenderer initialized successfully");
    return true;
}

void
TextRenderer::shutdown()
{
    if (!m_initialized)
        return;

    Logger::info("Shutting down TextRenderer...");

    if (m_fontLoaded)
    {
        UnloadFont(m_font);
        m_fontLoaded = false;
    }

    m_initialized = false;
}

bool
TextRenderer::loadFont(const String& fontPath, uint32 fontSize)
{
    Logger::info("Loading font: {} (size: {})", fontPath, fontSize);

    // Unload previous font if any
    if (m_fontLoaded)
    {
        UnloadFont(m_font);
        m_fontLoaded = false;
    }

    // Load font with Raylib
    m_font     = LoadFontEx(fontPath.c_str(), static_cast<int>(fontSize), nullptr, 0);
    m_fontSize = static_cast<float32>(fontSize);

    // Check if font loaded successfully
    if (m_font.texture.id == 0)
    {
        Logger::error("Failed to load font: {}", fontPath);
        return false;
    }

    m_fontLoaded = true;
    Logger::info("Font loaded successfully: {}", fontPath);
    return true;
}

void
TextRenderer::renderText(const String& text, float32 x, float32 y, float32 scale,
                         const glm::vec3& color)
{
    if (!m_initialized || !m_fontLoaded)
        return;

    Color raylibColor = toRaylib(color);
    float32 fontSize  = m_fontSize * scale;
    Vector2 position  = {x, y};

    DrawTextEx(m_font, text.c_str(), position, fontSize, 1.0f, raylibColor);
}

void
TextRenderer::renderTextWithCallback(
    const String& text, float32 x, float32 y, float32 scale, const glm::vec3& color,
    std::function<void(uint32, uint32, float32&, float32&, glm::vec3&, bool&)> charCallback)
{
    if (!m_initialized || !m_fontLoaded)
        return;

    uint32 charCount = static_cast<uint32>(text.length());
    uint32 charIndex = 0;
    float32 fontSize = m_fontSize * scale;

    float32 currentX = x;

    // Iterate through all characters
    for (char c : text)
    {
        // Get character metrics
        int codepoint      = static_cast<int>(c);
        GlyphInfo glyphInfo = GetGlyphInfo(m_font, codepoint);

        float32 charX = currentX;
        float32 charY = y;

        // Apply per-character callback
        glm::vec3 charColor = color;
        bool visible        = true;

        if (charCallback)
        {
            charCallback(charIndex, charCount, charX, charY, charColor, visible);
        }

        if (visible)
        {
            Color raylibColor = toRaylib(charColor);
            Vector2 position  = {charX, charY};

            // Render single character
            DrawTextCodepoint(m_font, codepoint, position, fontSize, raylibColor);
        }

        // Advance position for next character
        // Scale factor accounts for the base font size
        float32 advance = static_cast<float32>(glyphInfo.advanceX);
        if (advance == 0.0f)
        {
            advance = static_cast<float32>(glyphInfo.image.width);
        }
        currentX += advance * scale;

        charIndex++;
    }
}

void
TextRenderer::updateScreenSize(int32 width, int32 height)
{
    m_screenWidth  = width;
    m_screenHeight = height;

    Logger::debug("TextRenderer screen size updated: {}x{}", width, height);
}

float32
TextRenderer::getTextWidth(const String& text, float32 scale) const
{
    if (!m_fontLoaded)
        return 0.0f;

    float32 fontSize = m_fontSize * scale;
    Vector2 measured = MeasureTextEx(m_font, text.c_str(), fontSize, 1.0f);
    return measured.x;
}

float32
TextRenderer::getCharWidth(float32 scale) const
{
    if (!m_fontLoaded)
        return 0.0f;

    // Use 'M' as reference for character width (widest common character)
    int codepoint      = static_cast<int>('M');
    GlyphInfo glyphInfo = GetGlyphInfo(m_font, codepoint);

    float32 width = static_cast<float32>(glyphInfo.advanceX);
    if (width == 0.0f)
    {
        width = static_cast<float32>(glyphInfo.image.width);
    }

    return width * scale;
}

float32
TextRenderer::getLineHeight(float32 scale) const
{
    if (!m_fontLoaded)
        return 0.0f;

    // Use font's base size with some line spacing (1.2x)
    return m_fontSize * scale * 1.2f;
}

}  // namespace deadcode
