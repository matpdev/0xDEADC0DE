/**
 * @file MenuFrame.cpp
 * @brief Implementation of MenuFrame class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-22
 */

#include "deadcode/ui/MenuFrame.hpp"

#include "deadcode/core/Logger.hpp"
#include "deadcode/graphics/TextRenderer.hpp"

namespace deadcode
{

MenuFrame::MenuFrame()
{
    Logger::debug("Creating MenuFrame with default style");
    updateBorderChars();
}

MenuFrame::MenuFrame(FrameStyle style) : m_style(style)
{
    Logger::debug("Creating MenuFrame with custom style: {}", static_cast<int32>(style));
    updateBorderChars();
}

MenuFrame::~MenuFrame() = default;

void
MenuFrame::setStyle(FrameStyle style)
{
    m_style = style;
    updateBorderChars();
}

void
MenuFrame::setDimensions(float32 x, float32 y, int32 width, int32 height)
{
    Logger::debug("Setting MenuFrame dimensions: pos=({}, {}), size={}x{}", x, y, width, height);
    m_x      = x;
    m_y      = y;
    m_width  = width;
    m_height = height;
}

void
MenuFrame::setTitle(const String& title, FrameAlign align)
{
    Logger::debug("Setting MenuFrame title: '{}', align={}", title, static_cast<int32>(align));
    m_title      = title;
    m_titleAlign = align;
}

void
MenuFrame::setFooter(const String& footer, FrameAlign align)
{
    m_footer      = footer;
    m_footerAlign = align;
}

void
MenuFrame::setColors(const glm::vec3& borderColor, const glm::vec3& titleColor,
                     const glm::vec3& contentColor)
{
    m_borderColor  = borderColor;
    m_titleColor   = titleColor;
    m_contentColor = contentColor;
}

void
MenuFrame::setGlow(bool enable)
{
    m_glowEnabled = enable;
}

void
MenuFrame::setPadding(int32 padding)
{
    m_padding = padding;
}

void
MenuFrame::render(TextRenderer* textRenderer, float32 scale)
{
    if (!textRenderer)
    {
        Logger::warn("MenuFrame::render called with null TextRenderer");
        return;
    }
    Logger::trace("Rendering MenuFrame at ({}, {}) with scale {}", m_x, m_y, scale);

    float32 charWidth  = getCharWidth(textRenderer, scale);
    float32 charHeight = getCharHeight(textRenderer, scale);

    // Calculate actual dimensions
    float32 frameWidth  = static_cast<float32>(m_width) * charWidth;
    float32 frameHeight = static_cast<float32>(m_height) * charHeight;

    float32 renderScale = (scale > 0.0f) ? scale : m_scale;
    Logger::trace("Rendering MenuFrame at ({}, {}) with scale {}", m_x, m_y, renderScale);

    // Render top border
    renderHorizontalLine(textRenderer, m_x, m_y, m_width, m_topLeft, m_horizontal, m_topRight,
                         scale);

    // Render title if present
    if (!m_title.empty())
    {
        float32 titleY = m_y - charHeight;
        float32 titleX = calculateTitleX(textRenderer, m_title, m_titleAlign, scale);
        textRenderer->renderText(m_title, titleX, titleY, scale, m_titleColor);

        // Render separator below title
        float32 sepY = m_y - (charHeight * 2.0f);
        renderHorizontalLine(textRenderer, m_x, sepY, m_width, m_teeRight, m_horizontal, m_teeLeft,
                             scale);
    }

    // Render vertical borders
    renderVerticalLines(textRenderer, scale);

    // Render bottom border
    float32 bottomY = m_y - frameHeight;
    if (!m_footer.empty())
    {
        // Render separator above footer
        float32 sepY = bottomY - charHeight;
        renderHorizontalLine(textRenderer, m_x, sepY, m_width, m_teeRight, m_horizontal, m_teeLeft,
                             scale);

        // Render footer
        float32 footerX = calculateAlignedX(textRenderer, m_footer, m_footerAlign, scale);
        textRenderer->renderText(m_footer, footerX, sepY - charHeight, scale, m_contentColor);

        bottomY = sepY - (charHeight * 2.0f);
    }

    renderHorizontalLine(textRenderer, m_x, bottomY, m_width, m_bottomLeft, m_horizontal,
                         m_bottomRight, scale);
}

void
MenuFrame::renderText(TextRenderer* textRenderer, const String& text, int32 offsetY,
                      FrameAlign align, float32 scale)
{
    if (!textRenderer || text.empty())
    {
        if (!textRenderer)
            Logger::warn("MenuFrame::renderText called with null TextRenderer");
        return;
    }
    Logger::trace("Rendering text in frame: '{}' at offset Y={}, align={}", text, offsetY,
                  static_cast<int32>(align));

    float32 contentX, contentY;
    int32 contentWidth, contentHeight;
    getContentArea(textRenderer, scale, contentX, contentY, contentWidth, contentHeight);

    float32 charHeight = getCharHeight(textRenderer, scale);
    float32 textX      = calculateAlignedX(textRenderer, text, align, scale);
    float32 textY      = contentY - (static_cast<float32>(offsetY) * charHeight);

    textRenderer->renderText(text, textX, textY, scale, m_contentColor);
}

void
MenuFrame::renderLines(TextRenderer* textRenderer, const std::vector<String>& lines,
                       int32 startOffsetY, int32 lineSpacing, FrameAlign align, float32 scale)
{
    if (!textRenderer || lines.empty())
        return;

    int32 currentOffset = startOffsetY;
    for (const auto& line : lines)
    {
        renderText(textRenderer, line, currentOffset, align, scale);
        currentOffset += lineSpacing;
    }
}

void
MenuFrame::getContentArea(TextRenderer* textRenderer, float32 scale, float32& outX, float32& outY,
                          int32& outWidth, int32& outHeight) const
{
    outWidth  = m_width - (2 + m_padding * 2);   // Remove borders and padding
    outHeight = m_height - (2 + m_padding * 2);  // Remove borders and padding
    Logger::trace("Calculating content area: width={}, height={}, padding={}", outWidth, outHeight,
                  m_padding);

    float32 charWidth  = getCharWidth(textRenderer,
                                      scale);  // Approximate, should query from renderer
    float32 charHeight = getCharHeight(textRenderer, scale);  // Approximate

    outX = m_x + ((1 + m_padding) * charWidth);
    outY = m_y - ((1 + m_padding) * charHeight * 2.0f);

    if (!m_title.empty())
    {
        outY -= (charHeight * 2.0f);  // Account for title and separator
        outHeight -= 2;
    }

    if (!m_footer.empty())
    {
        outHeight -= 2;  // Account for footer and separator
    }
}

float32
MenuFrame::getCharWidth(TextRenderer* textRenderer, float32 scale) const
{
    if (textRenderer)
    {
        float32 width = textRenderer->getCharWidth(scale);
        Logger::trace("Character width at scale {}: {}", scale, width);
        return width;
    }
    Logger::debug("Using fallback char width estimate for scale {}", scale);
    return 16.0f * scale;  // Fallback estimate
}

float32
MenuFrame::getCharHeight(TextRenderer* textRenderer, float32 scale) const
{
    if (textRenderer)
    {
        float32 height = textRenderer->getLineHeight(scale);
        Logger::trace("Line height at scale {}: {}", scale, height);
        return height;
    }
    Logger::debug("Using fallback line height estimate for scale {}", scale);
    return 32.0f * scale;
}

void
MenuFrame::updateBorderChars()
{
    switch (m_style)
    {
        case FrameStyle::SINGLE:
            m_topLeft     = '\xDA';  // ┌
            m_topRight    = '\xBF';  // ┐
            m_bottomLeft  = '\xC0';  // └
            m_bottomRight = '\xD9';  // ┘
            m_horizontal  = '\xC4';  // ─
            m_vertical    = '\xB3';  // │
            m_teeDown     = '\xC2';  // ┬
            m_teeUp       = '\xC1';  // ┴
            m_teeRight    = '\xC3';  // ├
            m_teeLeft     = '\xB4';  // ┤
            break;

        case FrameStyle::DOUBLE:
            m_topLeft     = '\xC9';  // ╔
            m_topRight    = '\xBB';  // ╗
            m_bottomLeft  = '\xC8';  // ╚
            m_bottomRight = '\xBC';  // ╝
            m_horizontal  = '\xCD';  // ═
            m_vertical    = '\xBA';  // ║
            m_teeDown     = '\xCB';  // ╦
            m_teeUp       = '\xCA';  // ╩
            m_teeRight    = '\xCC';  // ╠
            m_teeLeft     = '\xB9';  // ╣
            break;

        case FrameStyle::HEAVY:
            m_topLeft     = '\xDA';  // Use single as fallback
            m_topRight    = '\xBF';
            m_bottomLeft  = '\xC0';
            m_bottomRight = '\xD9';
            m_horizontal  = '\xCD';  // Heavy horizontal
            m_vertical    = '\xBA';  // Heavy vertical
            m_teeDown     = '\xC2';
            m_teeUp       = '\xC1';
            m_teeRight    = '\xC3';
            m_teeLeft     = '\xB4';
            break;

        case FrameStyle::ROUNDED:
            // Use standard box drawing with '+' for rounded effect
            m_topLeft     = '+';
            m_topRight    = '+';
            m_bottomLeft  = '+';
            m_bottomRight = '+';
            m_horizontal  = '-';
            m_vertical    = '|';
            m_teeDown     = '+';
            m_teeUp       = '+';
            m_teeRight    = '+';
            m_teeLeft     = '+';
            break;

        case FrameStyle::CYBER:
            // Aggressive cyberpunk style
            m_topLeft     = '\xDA';
            m_topRight    = '\xBF';
            m_bottomLeft  = '\xC0';
            m_bottomRight = '\xD9';
            m_horizontal  = '\xC4';
            m_vertical    = '\xB3';
            m_teeDown     = '\xC2';
            m_teeUp       = '\xC1';
            m_teeRight    = '\xC3';
            m_teeLeft     = '\xB4';
            break;

        case FrameStyle::NEON:
            // Same as double for now, but with glow enabled
            m_topLeft     = '\xC9';
            m_topRight    = '\xBB';
            m_bottomLeft  = '\xC8';
            m_bottomRight = '\xBC';
            m_horizontal  = '\xCD';
            m_vertical    = '\xBA';
            m_teeDown     = '\xCB';
            m_teeUp       = '\xCA';
            m_teeRight    = '\xCC';
            m_teeLeft     = '\xB9';
            m_glowEnabled = true;
            break;
    }
}

void
MenuFrame::renderHorizontalLine(TextRenderer* textRenderer, float32 x, float32 y, int32 width,
                                char left, char middle, char right, float32 scale)
{
    if (!textRenderer)
        return;

    float32 charWidth = getCharWidth(textRenderer, scale);

    // Render left corner
    String leftStr(1, left);
    textRenderer->renderText(leftStr, x, y, scale, m_borderColor);

    // Render middle section
    String middleStr(width - 2, middle);
    textRenderer->renderText(middleStr, x + charWidth, y, scale, m_borderColor);

    // Render right corner
    String rightStr(1, right);
    textRenderer->renderText(rightStr, x + (charWidth * static_cast<float32>(width - 1)), y, scale,
                             m_borderColor);
}

void
MenuFrame::renderVerticalLines(TextRenderer* textRenderer, float32 scale)
{
    if (!textRenderer)
        return;

    float32 charWidth  = getCharWidth(textRenderer, scale);
    float32 charHeight = getCharHeight(textRenderer, scale);

    String vertStr(1, m_vertical);

    int32 startLine = 1;
    int32 endLine   = m_height - 1;

    // Adjust for title
    if (!m_title.empty())
    {
        startLine += 2;  // Skip title and separator
    }

    // Adjust for footer
    if (!m_footer.empty())
    {
        endLine -= 2;  // Skip separator and footer
    }

    // Render left and right vertical lines
    for (int32 i = startLine; i < endLine; ++i)
    {
        float32 lineY = m_y - (static_cast<float32>(i) * charHeight);

        // Left border
        textRenderer->renderText(vertStr, m_x, lineY, scale, m_borderColor);

        // Right border
        float32 rightX = m_x + (charWidth * static_cast<float32>(m_width - 1));
        textRenderer->renderText(vertStr, rightX, lineY, scale, m_borderColor);
    }
}

float32
MenuFrame::calculateAlignedX(TextRenderer* textRenderer, const String& text, FrameAlign align,
                             float32 scale) const
{
    if (!textRenderer)
    {
        Logger::warn("calculateAlignedX called with null TextRenderer");
        return m_x;
    }

    float32 charWidth  = getCharWidth(textRenderer, scale);
    float32 textWidth  = textRenderer->getTextWidth(text, scale);
    float32 frameWidth = static_cast<float32>(m_width) * charWidth;

    switch (align)
    {
        case FrameAlign::LEFT:
            return m_x + ((1 + m_padding) * charWidth);

        case FrameAlign::CENTER:
            return m_x + (frameWidth - textWidth) / 2.0f;

        case FrameAlign::RIGHT:
            return m_x + frameWidth - textWidth - ((1 + m_padding) * charWidth);

        default:
            return m_x;
    }
}

float32
MenuFrame::calculateDynamicScale(int32 windowWidth, int32 windowHeight, float32 baseScale) const
{
    constexpr int32 REFERENCE_WIDTH  = 1920;
    constexpr int32 REFERENCE_HEIGHT = 1080;

    float32 scaleX = static_cast<float32>(windowWidth) / REFERENCE_WIDTH;
    float32 scaleY = static_cast<float32>(windowHeight) / REFERENCE_HEIGHT;
    float32 scale  = std::min(scaleX, scaleY) * baseScale;

    Logger::debug("Calculated dynamic scale: window={}x{}, scale={}", windowWidth, windowHeight,
                  scale);

    return scale;
}

void
MenuFrame::setScale(float32 scale)
{
    Logger::debug("Setting MenuFrame scale: {}", scale);
    m_scale = scale;
}

void
MenuFrame::setScreenDimensions(int32 screenWidth, int32 screenHeight)
{
    Logger::debug("Setting MenuFrame screen dimensions: {}x{}", screenWidth, screenHeight);
    m_screenWidth  = screenWidth;
    m_screenHeight = screenHeight;
}

float32
MenuFrame::calculateTitleX(TextRenderer* textRenderer, const String& text, FrameAlign align,
                           float32 scale) const
{
    if (!textRenderer)
    {
        Logger::warn("calculateTitleX called with null TextRenderer");
        return m_x;
    }

    float32 textWidth = textRenderer->getTextWidth(text, scale);

    // If screen dimensions are set and alignment is CENTER, center on screen
    if (m_screenWidth > 0 && align == FrameAlign::CENTER)
    {
        return (static_cast<float32>(m_screenWidth) - textWidth) / 2.0f;
    }

    // Fall back to frame-based alignment
    return calculateAlignedX(textRenderer, text, align, scale);
}

}  // namespace deadcode
