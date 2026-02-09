/**
 * @file TextBox.hpp
 * @brief Manage TextBox Modals
 *
 * A simple modal to show a condition to you accept
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-07
 */

#pragma once

#include "deadcode/core/Types.hpp"
#include "deadcode/graphics/TextRenderer.hpp"
#include "deadcode/input/InputManager.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <raylib.h>

namespace deadcode
{
class TextRenderer;
class InputManager;

enum class BoxStyle
{
    SINGLE,   ///< Single line borders
    DOUBLE,   ///< Double line borders
    HEAVY,    ///< Heavy/thick borders
    ROUNDED,  ///< Rounded corners
    CYBER,    ///< Cyberpunk style with glitch effects
    NEON      ///< Neon glow style
};

class TextBox
{
public:
    TextBox();

    ~TextBox();

    bool initialize(int32 screenWidth, int32 screenHeight);

    void update(float deltaTime);

    void render(TextRenderer* textRenderer, float32 scale = 1.0f);

    void handleInput(int32 key, int32 action);

    void setTextTitle(String text);

    void setTextButtons(String false_op, String true_op);

    void setCallback(bool option, std::function<void()> callback);

    void setStyle(BoxStyle boxStyle);

    void setDimensions(float32 x, float32 y, int32 width, int32 height);

    void setColors(const glm::vec3& borderColor, const glm::vec3& titleColor,
                   const glm::vec3& buttonColor);

    float32 calculateDynamicScale(int32 windowWidth, int32 windowHeight,
                                  float32 baseScale = 1.0f) const;

    void setScreenDimensions(int32 screenWidth, int32 screenHeight);

    void setScale(float32 scale);

    void
    setVisible(bool visible)
    {
        m_visible = visible;
    }

    bool
    isVisible() const
    {
        return m_visible;
    }

    void onWindowResize(int32 screenWidth, int32 screenHeight);

private:
    void moveSelectionLeft();

    void moveSelectionRight();

    void executeSelection();

    BoxStyle m_boxStyle{BoxStyle::CYBER};
    int32 m_boxWidth{400};
    int32 m_boxHeight{200};
    int32 m_screenWidth{1920};
    int32 m_screenHeight{1080};

    bool m_visible{false};
    bool m_selectedOption{false};
    bool m_blinkState{true};

    float32 m_blinkTimerSelected{0.0f};
    float32 m_blinkTimer{0.0f};
    float32 m_scale{1.0f};
    Rectangle m_boxRectangle;
    Rectangle m_boxSelection;

    String m_title{""};
    String m_buttonText[2];

    std::function<void()> m_callbacks[static_cast<size_t>(2)];
};
}  // namespace deadcode
