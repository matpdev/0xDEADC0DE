/**
 * @file InputManager.cpp
 * @brief Implementation of the InputManager class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-22
 */

#include "deadcode/input/InputManager.hpp"

#include "deadcode/core/Logger.hpp"
#include "deadcode/graphics/Window.hpp"

#include <GLFW/glfw3.h>

namespace deadcode
{

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

    m_window = window->getNativeWindow();
    if (!m_window)
    {
        Logger::error("Invalid GLFW window");
        return false;
    }

    // Set user pointer to this instance for callbacks
    glfwSetWindowUserPointer(m_window, this);

    // Setup callbacks
    glfwSetKeyCallback(m_window, glfwKeyCallback);
    glfwSetCursorPosCallback(m_window, glfwCursorPosCallback);
    glfwSetMouseButtonCallback(m_window, glfwMouseButtonCallback);

    m_initialized = true;
    Logger::info("Input manager initialized");
    return true;
}

void
InputManager::shutdown()
{
    Logger::info("Shutting down input manager...");

    if (m_window)
    {
        glfwSetKeyCallback(m_window, nullptr);
        glfwSetCursorPosCallback(m_window, nullptr);
        glfwSetMouseButtonCallback(m_window, nullptr);
        m_window = nullptr;
    }

    m_initialized = false;
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

    Logger::info("X: {}, Y: {}", x, y);
}

void
InputManager::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager && inputManager->m_keyCallback)
    {
        inputManager->m_keyCallback(key, scancode, action, mods);
    }
}

void
InputManager::glfwCursorPosCallback(GLFWwindow* window, double x, double y)
{
    auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager)
    {
        inputManager->m_mouseX = x;
        inputManager->m_mouseY = y;

        if (inputManager->m_mouseMoveCallback)
        {
            inputManager->m_mouseMoveCallback(x, y);
        }
    }
}

void
InputManager::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager && inputManager->m_mouseButtonCallback)
    {
        inputManager->m_mouseButtonCallback(button, action, mods);
    }
}

}  // namespace deadcode
