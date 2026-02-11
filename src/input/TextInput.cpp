#include "deadcode/input/TextInput.hpp"

#include <raylib.h>

namespace deadcode
{
TextInput::TextInput(int32 width, int32 height)
    : m_cursorPosition(0.0F),
      m_cursorBlinkTime(0.0F),
      m_cursorVisible(true),
      m_width(width),
      m_height(height)
{
}

TextInput::~TextInput() = default;

void
TextInput::update(float deltaTime)
{
    m_cursorBlinkTime += deltaTime;
    if (m_cursorBlinkTime >= 0.5F)
    {
        m_cursorVisible   = !m_cursorVisible;
        m_cursorBlinkTime = 0.0F;
    }
}

void
TextInput::render(int32 posX, int32 posY)
{
    DrawRectangleLines(posX, posY, m_width, m_height, Color(255, 255, 255, 255));
    if (m_cursorVisible)
    {
        DrawRectangle(posX + 20, posY + ((m_height / 2) / 2), 5, m_height * 0.6,
                      Color(0, 0, 255, 255));
    }
}
}  // namespace deadcode
