#pragma once

#include "deadcode/core/Types.hpp"
#include "deadcode/input/InputManager.hpp"

namespace deadcode
{
class TextInput
{
public:
    TextInput(int32 width, int32 height);
    ~TextInput();

    void update(float deltaTime);
    void render(int32 posX, int32 posY);

private:
    std::string m_text;
    float m_cursorPosition;
    float m_cursorBlinkTime;
    bool m_cursorVisible;

    int32 m_width;
    int32 m_height;
};
}  // namespace deadcode
