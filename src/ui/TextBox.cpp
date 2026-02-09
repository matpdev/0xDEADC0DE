/**
 * @file TextBox.cpp
 * @brief Implementation of the TextBox class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-07
 */

#include "deadcode/ui/TextBox.hpp"

#include "deadcode/core/Logger.hpp"
#include "deadcode/core/Types.hpp"
#include "deadcode/graphics/TextRenderer.hpp"

#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>

#include <algorithm>
#include <cstddef>
#include <utility>

#include <raylib.h>

namespace deadcode
{
TextBox::TextBox() {}

TextBox::~TextBox() = default;

void
TextBox::update(float deltaTime)
{
    if (!m_visible)
    {
        return;
    }

    if (m_blinkTimer >= 0.5F)
    {
        m_blinkTimer = 0.0F;
        m_blinkState = !m_blinkState;
    }

    m_blinkTimer += deltaTime;
}

void
TextBox::setTextTitle(String title)
{
    m_title = title;
}

void
TextBox::setTextButtons(String false_op, String true_op)
{
    m_buttonText[0] = std::move(false_op);
    m_buttonText[1] = std::move(true_op);
}

bool
TextBox::initialize(int32 screenWidth, int32 screenHeight)
{
    Logger::info("Initializing text box...");

    m_screenWidth  = screenWidth;
    m_screenHeight = screenHeight;

    m_boxRectangle = Rectangle{(static_cast<float32>(screenWidth) / 2.0F) - (m_boxWidth / 2.0F),
                               (static_cast<float32>(screenHeight) / 2.0F) - (m_boxHeight / 2.0F),
                               static_cast<float>(m_boxWidth), static_cast<float>(m_boxHeight)};

    m_boxSelection  = Rectangle{0, 0, 20, 20};
    m_buttonText[0] = "No";
    m_buttonText[1] = "Yes";
    m_title         = "Are you sure?";

    Logger::info("Text box initialized");

    return true;
}

void
TextBox::render(TextRenderer* textRenderer, float32 scale)
{
    if (!textRenderer || !m_visible)
    {
        return;
    }

    DrawRectangleRec(m_boxRectangle, Color(0, 250, 0, 255));
    float32 textHeight     = textRenderer->getLineHeight(0.5F);
    float32 textNotWidth   = textRenderer->getTextWidth(m_buttonText[0], 0.5F);
    float32 textYesWidth   = textRenderer->getTextWidth(m_buttonText[1], 0.5F);
    float32 textTitleWidth = textRenderer->getTextWidth(m_title, 0.5F);

    float32 positionY = m_boxRectangle.y + m_boxRectangle.height - textHeight - 20;

    if (m_blinkState)
    {
        if (m_selectedOption)
        {
            m_boxSelection.width  = textYesWidth + 10 + 2;
            m_boxSelection.height = textHeight + 2;
            m_boxSelection.x      = m_boxRectangle.x + 20 - 5;
            m_boxSelection.y      = positionY;
        }
        else
        {
            m_boxSelection.width  = textNotWidth + 10 + 2;
            m_boxSelection.height = textHeight + 2;
            m_boxSelection.x      = m_boxRectangle.x + m_boxRectangle.width - textNotWidth - 20 - 5;
            m_boxSelection.y      = positionY;
        }
        DrawRectangleLinesEx(m_boxSelection, 2, Color(100, 100, 100, 150));
    }

    textRenderer->renderText(m_buttonText[0],
                             m_boxRectangle.x + m_boxRectangle.width - textNotWidth - 20, positionY,
                             0.5F, glm::vec3(0.2f, 0.3f, 0.5f));

    textRenderer->renderText(m_buttonText[1], m_boxRectangle.x + 20, positionY, 0.5F,
                             glm::vec3(0.2f, 0.3f, 0.5f));

    textRenderer->renderText(
        m_title, m_boxRectangle.x + (m_boxRectangle.width / 2) - (textTitleWidth / 2),
        m_boxRectangle.y + m_boxRectangle.height * 0.2F, 0.5F, glm::vec3(0.2f, 0.3f, 0.5f));
}

float32
TextBox::calculateDynamicScale(int32 windowWidth, int32 windowHeight, float32 baseScale) const
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
TextBox::onWindowResize(int32 screenWidth, int32 screenHeight)
{
    m_screenWidth  = screenWidth;
    m_screenHeight = screenHeight;
}

void
TextBox::handleInput(int32 key, int32 action)
{
    if (!m_visible || action != GLFW_PRESS)
        return;

    switch (key)
    {
        case GLFW_KEY_LEFT:
        case GLFW_KEY_A:
            moveSelectionLeft();
            break;

        case GLFW_KEY_RIGHT:
        case GLFW_KEY_D:
            moveSelectionRight();
            break;

        case GLFW_KEY_ENTER:
        case GLFW_KEY_SPACE:
            executeSelection();
            break;

        case GLFW_KEY_ESCAPE:
            // ESC goes to exit
            m_visible = false;
            break;
    }
}

void
TextBox::moveSelectionLeft()
{
    m_selectedOption     = !m_selectedOption;
    m_blinkState         = true;
    m_blinkTimerSelected = 0.0F;
}

void
TextBox::moveSelectionRight()
{
    m_selectedOption     = !m_selectedOption;
    m_blinkState         = true;
    m_blinkTimerSelected = 0.0F;
}

void
TextBox::executeSelection()
{
    size_t index = static_cast<size_t>(m_selectedOption);
    if (m_callbacks[index])
    {
        m_callbacks[index]();
    }
}

void
TextBox::setCallback(bool option, std::function<void()> callback)
{
    size_t index = static_cast<size_t>(option);
    if (index < 2)
    {
        m_callbacks[index] = callback;
    }
}
}  // namespace deadcode
