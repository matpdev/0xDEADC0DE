/**
 * @file InputManager.cpp
 * @brief Implementation of the InputManager class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-08
 */

#include "deadcode/input/InputManager.hpp"

#include "deadcode/core/Logger.hpp"
#include "deadcode/graphics/Window.hpp"

#include <raylib.h>

namespace deadcode
{

// Raylib key mappings (compatible with GLFW where possible)
// These are already defined by Raylib, we just use them directly

InputManager::InputManager() = default;

InputManager::~InputManager()
{
    if (m_initialized)
    {
        shutdown();
    }
}

bool
InputManager::initialize(Window* window)
{
    Logger::info("Initializing input manager...");

    if (!window)
    {
        Logger::error("Invalid window pointer");
        return false;
    }

    m_window = window;

    m_initialized = true;
    Logger::info("Input manager initialized");
    return true;
}

void
InputManager::shutdown()
{
    Logger::info("Shutting down input manager...");

    m_window = nullptr;
    m_previousKeyStates.clear();
    m_previousMouseButtonStates.clear();

    m_initialized = false;
}

void
InputManager::pollEvents()
{
    if (!m_initialized)
        return;

    // Poll mouse position
    Vector2 mousePos = GetMousePosition();
    double newMouseX = static_cast<double>(mousePos.x);
    double newMouseY = static_cast<double>(mousePos.y);

    // Trigger mouse move callback if position changed
    if (newMouseX != m_mouseX || newMouseY != m_mouseY)
    {
        m_mouseX = newMouseX;
        m_mouseY = newMouseY;

        if (m_mouseMoveCallback)
        {
            m_mouseMoveCallback(m_mouseX, m_mouseY);
        }
    }

    // Poll keyboard events
    // Check commonly used keys for state changes
    static const int keysToCheck[] = {
        KEY_ESCAPE,     KEY_ENTER,      KEY_SPACE,      KEY_BACKSPACE,  KEY_TAB,
        KEY_UP,         KEY_DOWN,       KEY_LEFT,       KEY_RIGHT,      KEY_W,
        KEY_A,          KEY_S,          KEY_D,          KEY_LEFT_SHIFT, KEY_LEFT_CONTROL,
        KEY_LEFT_ALT,   KEY_F1,         KEY_F2,         KEY_F3,         KEY_F4,
        KEY_F5,         KEY_F6,         KEY_F7,         KEY_F8,         KEY_F9,
        KEY_F10,        KEY_F11,        KEY_F12,        KEY_ZERO,       KEY_ONE,
        KEY_TWO,        KEY_THREE,      KEY_FOUR,       KEY_FIVE,       KEY_SIX,
        KEY_SEVEN,      KEY_EIGHT,      KEY_NINE,       KEY_APOSTROPHE, KEY_COMMA,
        KEY_MINUS,      KEY_PERIOD,     KEY_SLASH,      KEY_SEMICOLON,  KEY_EQUAL,
        KEY_LEFT_BRACKET, KEY_BACKSLASH, KEY_RIGHT_BRACKET, KEY_GRAVE,  KEY_KP_0,
        KEY_KP_1,       KEY_KP_2,       KEY_KP_3,       KEY_KP_4,       KEY_KP_5,
        KEY_KP_6,       KEY_KP_7,       KEY_KP_8,       KEY_KP_9,       KEY_KP_DECIMAL,
        KEY_KP_DIVIDE,  KEY_KP_MULTIPLY, KEY_KP_SUBTRACT, KEY_KP_ADD,   KEY_KP_ENTER,
        KEY_KP_EQUAL
    };

    // Also check letter keys
    for (int key = KEY_A; key <= KEY_Z; ++key)
    {
        bool currentState  = IsKeyDown(key);
        bool previousState = m_previousKeyStates[key];

        if (currentState && !previousState)
        {
            // Key press
            if (m_keyCallback)
            {
                m_keyCallback(key, 0, 1, 0);  // action = 1 (press)
            }
        }
        else if (!currentState && previousState)
        {
            // Key release
            if (m_keyCallback)
            {
                m_keyCallback(key, 0, 0, 0);  // action = 0 (release)
            }
        }

        m_previousKeyStates[key] = currentState;
    }

    // Check special keys
    for (int key : keysToCheck)
    {
        bool currentState  = IsKeyDown(key);
        bool previousState = m_previousKeyStates[key];

        if (currentState && !previousState)
        {
            // Key press
            if (m_keyCallback)
            {
                m_keyCallback(key, 0, 1, 0);  // action = 1 (press)
            }
        }
        else if (!currentState && previousState)
        {
            // Key release
            if (m_keyCallback)
            {
                m_keyCallback(key, 0, 0, 0);  // action = 0 (release)
            }
        }

        m_previousKeyStates[key] = currentState;
    }

    // Poll mouse button events
    static const int mouseButtonsToCheck[] = {MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT,
                                              MOUSE_BUTTON_MIDDLE};

    for (int button : mouseButtonsToCheck)
    {
        bool currentState  = IsMouseButtonDown(button);
        bool previousState = m_previousMouseButtonStates[button];

        if (currentState && !previousState)
        {
            // Button press
            if (m_mouseButtonCallback)
            {
                m_mouseButtonCallback(button, 1, 0);  // action = 1 (press)
            }
        }
        else if (!currentState && previousState)
        {
            // Button release
            if (m_mouseButtonCallback)
            {
                m_mouseButtonCallback(button, 0, 0);  // action = 0 (release)
            }
        }

        m_previousMouseButtonStates[button] = currentState;
    }
}

void
InputManager::setKeyCallback(KeyCallback callback)
{
    m_keyCallback = callback;
}

void
InputManager::setMouseMoveCallback(MouseMoveCallback callback)
{
    m_mouseMoveCallback = callback;
}

void
InputManager::setMouseButtonCallback(MouseButtonCallback callback)
{
    m_mouseButtonCallback = callback;
}

void
InputManager::getMousePosition(double& x, double& y) const
{
    x = m_mouseX;
    y = m_mouseY;
}

}  // namespace deadcode
