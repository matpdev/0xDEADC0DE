/**
 * @file MenuFrame.hpp
 * @brief Beautiful ASCII art frames for menus
 *
 * Provides various frame styles for decorating menus with borders,
 * titles, and visual effects using box-drawing characters.
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-22
 */

#pragma once

#include "deadcode/core/Types.hpp"

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace deadcode
{

class TextRenderer;

/**
 * @brief Frame style types
 */
enum class FrameStyle
{
    SINGLE,   ///< Single line borders
    DOUBLE,   ///< Double line borders
    HEAVY,    ///< Heavy/thick borders
    ROUNDED,  ///< Rounded corners
    CYBER,    ///< Cyberpunk style with glitch effects
    NEON      ///< Neon glow style
};

/**
 * @brief Frame alignment options
 */
enum class FrameAlign
{
    LEFT,
    CENTER,
    RIGHT
};

/**
 * @brief Beautiful ASCII art frame renderer
 *
 * Draws decorative frames around content using box-drawing
 * characters. Supports multiple styles and customization.
 */
class MenuFrame
{
public:
    /**
     * @brief Constructor
     */
    MenuFrame();

    /**
     * @brief Constructor with style
     *
     * @param style Frame style to use
     */
    explicit MenuFrame(FrameStyle style);

    /**
     * @brief Destructor
     */
    ~MenuFrame();

    /**
     * @brief Set frame style
     *
     * @param style Frame style
     */
    void setStyle(FrameStyle style);

    /**
     * @brief Set frame dimensions
     *
     * @param x X position (screen coordinates)
     * @param y Y position (screen coordinates)
     * @param width Width in characters
     * @param height Height in characters
     */
    void setDimensions(float32 x, float32 y, int32 width, int32 height);

    /**
     * @brief Set frame title
     *
     * @param title Title text to display in frame
     * @param align Title alignment
     */
    void setTitle(const String& title, FrameAlign align = FrameAlign::CENTER);

    /**
     * @brief Set frame footer
     *
     * @param footer Footer text to display
     * @param align Footer alignment
     */
    void setFooter(const String& footer, FrameAlign align = FrameAlign::CENTER);

    /**
     * @brief Set frame colors
     *
     * @param borderColor Color for border lines
     * @param titleColor Color for title text
     * @param contentColor Default color for content
     */
    void setColors(const glm::vec3& borderColor, const glm::vec3& titleColor,
                   const glm::vec3& contentColor);

    /**
     * @brief Enable/disable glow effect
     *
     * @param enable Whether to enable glow
     */
    void setGlow(bool enable);

    /**
     * @brief Set padding inside frame
     *
     * @param padding Padding in character units
     */
    void setPadding(int32 padding);

    /**
     * @brief Render the frame
     *
     * @param textRenderer TextRenderer to use for drawing
     * @param scale Text scale factor
     */
    void render(TextRenderer* textRenderer, float32 scale = 1.0f);

    /**
     * @brief Render text inside the frame
     *
     * @param textRenderer TextRenderer to use
     * @param text Text to render
     * @param offsetY Y offset from top of frame content area
     * @param align Text alignment
     * @param scale Text scale
     */
    void renderText(TextRenderer* textRenderer, const String& text, int32 offsetY,
                    FrameAlign align = FrameAlign::LEFT, float32 scale = 1.0f);

    /**
     * @brief Render multiple lines inside the frame
     *
     * @param textRenderer TextRenderer to use
     * @param lines Lines of text to render
     * @param startOffsetY Starting Y offset
     * @param lineSpacing Spacing between lines
     * @param align Text alignment
     * @param scale Text scale
     */
    void renderLines(TextRenderer* textRenderer, const std::vector<String>& lines,
                     int32 startOffsetY, int32 lineSpacing = 1, FrameAlign align = FrameAlign::LEFT,
                     float32 scale = 1.0f);

    /**
     * @brief Get content area dimensions
     *
     * @param outX Output X position of content area
     * @param outY Output Y position of content area
     * @param outWidth Output width of content area
     * @param outHeight Output height of content area
     */
    void getContentArea(TextRenderer* textRenderer, float32 scale, float32& outX, float32& outY,
                        int32& outWidth, int32& outHeight) const;

    /**
     * @brief Get character width at current scale
     *
     * @param textRenderer TextRenderer to measure with
     * @param scale Text scale
     * @return Character width in pixels
     */
    float32 getCharWidth(TextRenderer* textRenderer, float32 scale = 1.0f) const;

    /**
     * @brief Get character height at current scale
     *
     * @param textRenderer TextRenderer to measure with
     * @param scale Text scale
     * @return Character height in pixels
     */
    float32 getCharHeight(TextRenderer* textRenderer, float32 scale = 1.0f) const;

    float32 calculateDynamicScale(int32 windowWidth, int32 windowHeight,
                                  float32 baseScale = 1.0f) const;

    void setScale(float32 scale);

    /**
     * @brief Set screen dimensions for proper centering
     *
     * @param screenWidth Screen width in pixels
     * @param screenHeight Screen height in pixels
     */
    void setScreenDimensions(int32 screenWidth, int32 screenHeight);

    float32 m_scale{1.0f};

private:
    /**
     * @brief Get border characters for current style
     */
    void updateBorderChars();

    /**
     * @brief Render horizontal line
     */
    void renderHorizontalLine(TextRenderer* textRenderer, float32 x, float32 y, int32 width,
                              char left, char middle, char right, float32 scale);

    /**
     * @brief Render vertical lines
     */
    void renderVerticalLines(TextRenderer* textRenderer, float32 scale);

    /**
     * @brief Calculate text position based on alignment
     */
    float32 calculateAlignedX(TextRenderer* textRenderer, const String& text, FrameAlign align,
                              float32 scale) const;

    /**
     * @brief Calculate title X position using screen dimensions
     */
    float32 calculateTitleX(TextRenderer* textRenderer, const String& text, FrameAlign align,
                            float32 scale) const;

    FrameStyle m_style{FrameStyle::SINGLE};
    float32 m_x{0.0f};
    float32 m_y{0.0f};

    int32 m_width{40};
    int32 m_height{20};
    int32 m_padding{1};
    bool m_glowEnabled{false};

    String m_title;
    FrameAlign m_titleAlign{FrameAlign::CENTER};
    String m_footer;
    FrameAlign m_footerAlign{FrameAlign::CENTER};

    glm::vec3 m_borderColor{0.0f, 1.0f, 1.0f};   // Cyan
    glm::vec3 m_titleColor{1.0f, 1.0f, 0.0f};    // Yellow
    glm::vec3 m_contentColor{1.0f, 1.0f, 1.0f};  // White

    int32 m_screenWidth{0};   // Screen width for centering (0 = use frame width)
    int32 m_screenHeight{0};  // Screen height for centering

    // Border characters for current style
    char m_topLeft;
    char m_topRight;
    char m_bottomLeft;
    char m_bottomRight;
    char m_horizontal;
    char m_vertical;
    char m_teeDown;   // T pointing down
    char m_teeUp;     // T pointing up
    char m_teeRight;  // T pointing right
    char m_teeLeft;   // T pointing left
};

}  // namespace deadcode
