/**
 * @file Window.cpp
 * @brief Implementation of Window class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-21
 */

#include "deadcode/graphics/Window.hpp"

#include "deadcode/core/Logger.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

namespace deadcode
{

Window::Window() : m_window(nullptr) {}

Window::~Window()
{
    close();
}

Window::Window(Window&& other) noexcept : m_window(other.m_window), m_config(other.m_config)
{
    other.m_window = nullptr;
}

Window&
Window::operator=(Window&& other) noexcept
{
    if (this != &other)
    {
        close();
        m_window       = other.m_window;
        m_config       = other.m_config;
        other.m_window = nullptr;
    }
    return *this;
}

bool
Window::create(const WindowConfig& config)
{
    m_config = config;

    Logger::info("Creating window: {}x{}", config.width, config.height);

    // Set OpenGL version hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.glMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.glMinorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    m_window = glfwCreateWindow(config.width, config.height, config.title.c_str(),
                                config.fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

    if (!m_window)
    {
        Logger::error("Failed to create GLFW window");
        return false;
    }

    // Make the window's context current
    glfwMakeContextCurrent(m_window);

    // Setup GLEW
    if (!setupOpenGL())
    {
        close();
        return false;
    }

    // Set vsync
    glfwSwapInterval(config.vsync ? 1 : 0);

    // Set callbacks
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    glfwSetWindowUserPointer(m_window, this);

    // Get actual framebuffer size (may differ from window size on high DPI)
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    Logger::info("Window created successfully");
    Logger::info("OpenGL Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    Logger::info("GLSL Version: {}",
                 reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    Logger::info("Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

    return true;
}

void
Window::close()
{
    if (m_window)
    {
        Logger::info("Closing window");
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

bool
Window::shouldClose() const
{
    return m_window && glfwWindowShouldClose(m_window);
}

void
Window::swapBuffers()
{
    if (m_window)
    {
        glfwSwapBuffers(m_window);
    }
}

int32
Window::getWidth() const
{
    if (!m_window)
        return 0;

    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return static_cast<int32>(width);
}

int32
Window::getHeight() const
{
    if (!m_window)
        return 0;

    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return static_cast<int32>(height);
}

float32
Window::getAspectRatio() const
{
    int32 height = getHeight();
    if (height == 0)
        return 0.0f;
    return static_cast<float32>(getWidth()) / static_cast<float32>(height);
}

bool
Window::isFullscreen() const
{
    return m_config.fullscreen;
}

void
Window::setTitle(const String& title)
{
    m_config.title = title;
    if (m_window)
    {
        glfwSetWindowTitle(m_window, title.c_str());
    }
}

GLFWwindow*
Window::getNativeWindow() const
{
    return m_window;
}

bool
Window::setupOpenGL()
{
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err       = glewInit();
    if (err != GLEW_OK)
    {
        Logger::error("Failed to initialize GLEW: {}",
                      reinterpret_cast<const char*>(glewGetErrorString(err)));
        return false;
    }

    // Check OpenGL version
    if (!GLEW_VERSION_3_3)
    {
        Logger::error("OpenGL 3.3 or higher is required");
        return false;
    }

    // Enable blending for text rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void
Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    (void) window;  // Unused
    glViewport(0, 0, width, height);
    Logger::debug("Framebuffer resized: {}x{}", width, height);
}

}  // namespace deadcode
