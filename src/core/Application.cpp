/**
 * @file Application.cpp
 * @brief Implementation of the Application class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-21
 */

#include "deadcode/core/Application.hpp"

#include "deadcode/core/Config.hpp"
#include "deadcode/core/Logger.hpp"
#include "deadcode/graphics/Renderer.hpp"
#include "deadcode/graphics/Window.hpp"

#include <GLFW/glfw3.h>

#include <chrono>
#include <thread>

namespace deadcode
{

// Pimpl implementation
struct Application::Impl
{
    UniquePtr<Config> config;
    UniquePtr<Window> window;
    UniquePtr<Renderer> renderer;

    std::chrono::high_resolution_clock::time_point lastFrameTime;
    float deltaTime{0.0f};
};

Application::Application()
    : m_impl(std::make_unique<Impl>()),
      m_initialized(false),
      m_running(false),
      m_exitRequested(false),
      m_targetFPS(60),
      m_currentFPS(0.0f)
{
}

Application::~Application()
{
    if (m_initialized)
    {
        shutdown();
    }
}

Application&
Application::getInstance()
{
    static Application instance;
    return instance;
}

bool
Application::initialize(int argc, char** argv)
{
    Logger::info("Initializing application...");

    // Initialize GLFW
    if (!glfwInit())
    {
        Logger::error("Failed to initialize GLFW");
        return false;
    }

    if (!initializeLogger())
    {
        return false;
    }

    if (!initializeConfig())
    {
        return false;
    }

    if (!initializeWindow())
    {
        return false;
    }

    if (!initializeRenderer())
    {
        return false;
    }

    if (!initializeInput())
    {
        return false;
    }

    if (!initializeAudio())
    {
        return false;
    }

    if (!initializeResources())
    {
        return false;
    }

    if (!initializeGame())
    {
        return false;
    }

    m_impl->lastFrameTime = std::chrono::high_resolution_clock::now();
    m_initialized         = true;

    Logger::info("Application initialization complete");
    return true;
}

void
Application::run()
{
    if (!m_initialized)
    {
        Logger::error("Cannot run application: not initialized");
        return;
    }

    Logger::info("Starting main game loop");
    m_running = true;

    while (m_running && !m_exitRequested && !m_impl->window->shouldClose())
    {
        auto currentTime                     = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - m_impl->lastFrameTime;
        m_impl->deltaTime                    = elapsed.count();
        m_impl->lastFrameTime                = currentTime;

        // Update FPS
        if (m_impl->deltaTime > 0.0f)
        {
            m_currentFPS = 1.0f / m_impl->deltaTime;
        }

        processInput(m_impl->deltaTime);
        update(m_impl->deltaTime);
        render(m_impl->deltaTime);
        syncFrameRate();

        glfwPollEvents();
    }

    m_running = false;
    Logger::info("Main game loop ended");
}

void
Application::shutdown()
{
    Logger::info("Shutting down application...");

    // Shutdown subsystems in reverse order
    // TODO: Implement subsystem shutdown

    glfwTerminate();

    m_initialized = false;
    Logger::info("Application shutdown complete");
}

void
Application::requestExit()
{
    Logger::info("Exit requested");
    m_exitRequested = true;
}

bool
Application::isExitRequested() const
{
    return m_exitRequested;
}

int
Application::getTargetFPS() const
{
    return m_targetFPS;
}

void
Application::setTargetFPS(int fps)
{
    m_targetFPS = fps;
    Logger::info("Target FPS set to: {}", fps);
}

float
Application::getCurrentFPS() const
{
    return m_currentFPS;
}

bool
Application::initializeLogger()
{
    Logger::info("Logger already initialized");
    return true;
}

bool
Application::initializeConfig()
{
    Logger::info("Initializing configuration...");

    m_impl->config = std::make_unique<Config>();

    if (!m_impl->config->load("assets/config/game.json"))
    {
        Logger::warn("Failed to load game.json, using defaults");
    }

    return true;
}

bool
Application::initializeWindow()
{
    Logger::info("Initializing window...");

    m_impl->window = std::make_unique<Window>();

    WindowConfig config;
    config.title  = "0xDEADC0DE - Text-Based RPG";
    config.width  = 800;
    config.height = 600;
    config.vsync  = true;

    if (!m_impl->window->create(config))
    {
        Logger::error("Failed to create window");
        return false;
    }

    return true;
}

bool
Application::initializeRenderer()
{
    Logger::info("Initializing renderer...");

    m_impl->renderer = std::make_unique<Renderer>();

    if (!m_impl->renderer->initialize(m_impl->window.get()))
    {
        Logger::error("Failed to initialize renderer");
        return false;
    }

    // Load font
    if (!m_impl->renderer->getTextRenderer()->loadFont("assets/fonts/dos-vga.ttf", 48))
    {
        Logger::error("Failed to load font");
        return false;
    }

    // Set clear color to dark blue
    m_impl->renderer->setClearColor(glm::vec3(0.0f, 0.0f, 0.2f));

    return true;
}

bool
Application::initializeInput()
{
    Logger::info("Initializing input system...");
    // TODO: Implement input initialization
    return true;
}

bool
Application::initializeAudio()
{
    Logger::info("Initializing audio system...");
    // TODO: Implement audio initialization
    return true;
}

bool
Application::initializeResources()
{
    Logger::info("Initializing resource manager...");
    // TODO: Implement resource manager initialization
    return true;
}

bool
Application::initializeGame()
{
    Logger::info("Initializing game systems...");
    // TODO: Implement game initialization
    return true;
}

void
Application::processInput(float deltaTime)
{
    // TODO: Implement input processing
}

void
Application::update(float deltaTime)
{
    // TODO: Implement game state update
}

void
Application::render(float deltaTime)
{
    if (!m_impl->renderer)
        return;

    m_impl->renderer->beginFrame();

    // Render "Hello World" in green at center of screen
    auto* textRenderer = m_impl->renderer->getTextRenderer();
    if (textRenderer)
    {
        float32 x     = 200.0f;
        float32 y     = 300.0f;
        float32 scale = 1.0f;
        glm::vec3 color(0.0f, 1.0f, 0.0f);  // Green

        textRenderer->renderText("Hello World", x, y, scale, color);
    }

    m_impl->renderer->endFrame();
}

void
Application::syncFrameRate()
{
    if (m_targetFPS <= 0)
    {
        return;  // Unlimited FPS
    }

    auto targetFrameTime                   = std::chrono::duration<float>(1.0f / m_targetFPS);
    auto currentTime                       = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> frameTime = currentTime - m_impl->lastFrameTime;

    if (frameTime < targetFrameTime)
    {
        auto sleepTime = targetFrameTime - frameTime;
        std::this_thread::sleep_for(sleepTime);
    }
}

}  // namespace deadcode
