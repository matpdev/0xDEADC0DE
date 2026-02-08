/**
 * @file TextBox.hpp
 * @brief Manage TextBox Modals
 *
 * A simple modal to show a condition to you accept
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-0
 */

#pragma once

#include "deadcode/core/Types.hpp"
#include "deadcode/graphics/TextRenderer.hpp"
#include "deadcode/input/InputManager.hpp"
#include "deadcode/ui/MenuFrame.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace deadcode
{
class TextRenderer;
class InputManager;

class TextBox
{
public:
    TextBox();

    ~TextBox();

    bool initialize(int32 boxWidth, int32 boxHeight);

    void update(float deltaTime);

    void render(TextRenderer* textRenderer);

    void handleInput(int32 key, int32 action);

    void setTextTitle(String text);
    void setTextButtons(String false_op, String true_op);

    void setCallback(bool option, std::function<void()> callback);

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
    int32 m_boxWidth{200};
    int32 m_boxHeight{200};
    bool m_visible{true};

    bool m_selectedOption{false};
    std::function<void()> m_callbacks[static_cast<size_t>(2)];

    float32 m_blinkTimerSelected{0.0f};
    bool m_blinkState{true};
    float32 m_glitchTimer{0.0f};
};
}  // namespace deadcode
