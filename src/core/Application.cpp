/**
 * @file Application.cpp
 * @brief Implementation of the Application class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-21
 */

#include "deadcode/core/Application.hpp"

#include "deadcode/audio/AudioManager.hpp"
#include "deadcode/core/Config.hpp"
#include "deadcode/core/Logger.hpp"
#include "deadcode/core/Types.hpp"
#include "deadcode/core/Version.hpp"
#include "deadcode/game/GameLoop.hpp"
#include "deadcode/game/GameState.hpp"
#include "deadcode/game/SaveSystem.hpp"
#include "deadcode/graphics/Renderer.hpp"
#include "deadcode/graphics/Window.hpp"
#include "deadcode/input/InputManager.hpp"
#include "deadcode/ui/StartMenu.hpp"
#include "deadcode/ui/TextBox.hpp"

#include <chrono>
#include <memory>
#include <thread>

#include <raylib.h>

namespace deadcode
{

// Pimpl implementation
struct Application::Impl
{
    UniquePtr<Config> config;
    UniquePtr<Window> window;
    UniquePtr<Renderer> renderer;
    UniquePtr<InputManager> inputManager;
    UniquePtr<AudioManager> audioManager;
    UniquePtr<StartMenu> mainMenu;
    UniquePtr<SaveSystem> saveSystem;
    UniquePtr<TextBox> textBox;
    UniquePtr<GameLoop> gameLoop;

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
    (void) argc;  // Unused for now
    (void) argv;  // Unused for now

    Logger::info("Initializing application...");

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

    if (!initializeGameLoop())
    {
        return false;
    }

    if (!initializeTextBox())
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

        // Check for window resize
        handleWindowResize();

        // Poll input events (Raylib uses polling instead of callbacks)
        if (m_impl->inputManager)
        {
            m_impl->inputManager->pollEvents();
        }

        processInput(m_impl->deltaTime);
        update(m_impl->deltaTime);
        render(m_impl->deltaTime);

        // Raylib handles frame timing internally via SetTargetFPS
        // No need for manual syncFrameRate
    }

    m_running = false;
    Logger::info("Main game loop ended");
}

void
Application::shutdown()
{
    Logger::info("Shutting down application...");

    // Shutdown subsystems in reverse order
    if (m_impl->audioManager)
    {
        m_impl->audioManager->shutdown();
    }

    if (m_impl->inputManager)
    {
        m_impl->inputManager->shutdown();
    }

    if (m_impl->renderer)
    {
        m_impl->renderer->shutdown();
    }

    if (m_impl->window)
    {
        m_impl->window->close();
    }

    m_initialized = false;
    Logger::info("Application shutdown complete");
}

void
Application::requestExit()
{
    Logger::info("Exit requested");

    // m_exitRequested = true;

    m_impl->textBox->setVisible(true);

    m_impl->textBox->setCallback(false, [this]() -> void { m_impl->textBox->setVisible(false); });
    m_impl->textBox->setCallback(true, [this]() -> void { m_exitRequested = true; });
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
    config.title     = Version::getGameTitleWithVersion() + " - Text-Based RPG";
    config.width     = 800;
    config.height    = 600;
    config.vsync     = true;
    config.targetFPS = m_targetFPS;

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
    if (!m_impl->renderer->getTextRenderer()->loadFont("assets/fonts/PixelOperator-Bold.ttf", 52))
    {
        Logger::error("Failed to load font");
        return false;
    }

    // Set clear color to dark blue
    m_impl->renderer->setClearColor(glm::vec3(0.0f, 0.0f, 0.0f));

    return true;
}

bool
Application::initializeInput()
{
    Logger::info("Initializing input system...");

    m_impl->inputManager = std::make_unique<InputManager>();

    if (!m_impl->inputManager->initialize(m_impl->window.get()))
    {
        Logger::error("Failed to initialize input manager");
        return false;
    }

    // Setup input callbacks
    m_impl->inputManager->setKeyCallback([this](int key, int scancode, int action, int mods) {
        handleKeyInput(key, scancode, action, mods);
    });

    m_impl->inputManager->setMouseMoveCallback(
        [this](double x, double y) { handleMouseMove(x, y); });

    m_impl->inputManager->setMouseButtonCallback(
        [this](int button, int action, int mods) { handleMouseButton(button, action, mods); });

    return true;
}

bool
Application::initializeAudio()
{
    Logger::info("Initializing audio system...");

    m_impl->audioManager = std::make_unique<AudioManager>();

    if (!m_impl->audioManager->initialize())
    {
        Logger::error("Failed to initialize audio manager");
        return false;
    }

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

    // Initialize save system
    m_impl->saveSystem = std::make_unique<SaveSystem>();
    if (!m_impl->saveSystem->initialize())
    {
        Logger::error("Failed to initialize save system");
        return false;
    }

    // Initialize start menu
    m_impl->mainMenu = std::make_unique<StartMenu>();
    if (!m_impl->mainMenu->initialize(m_impl->window->getWidth(), m_impl->window->getHeight()))
    {
        Logger::error("Failed to initialize start menu");
        return false;
    }

    // Setup menu callbacks
    setupMainMenu();

    return true;
}

bool
Application::initializeTextBox()
{
    Logger::info("Initializing text box systems...");

    m_impl->textBox = std::make_unique<TextBox>();
    if (!m_impl->textBox->initialize(m_impl->window->getWidth(), m_impl->window->getHeight()))
    {
        return false;
    }

    return true;
}

bool
Application::initializeGameLoop()
{
    Logger::info("Initializing game loop...");

    m_impl->gameLoop = std::make_unique<GameLoop>();
    if (!m_impl->gameLoop->initialize(m_impl->window->getWidth(), m_impl->window->getHeight()))
    {
        return false;
    }

    return true;
}

void
Application::processInput(float deltaTime)
{
    (void) deltaTime;  // Unused for now

    // Input is handled via callbacks (now triggered by pollEvents)
    // Check for ESC key to exit
    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (m_gameState == GameState::Playing)
        {
            // Return to main menu from game
            m_gameState = GameState::MainMenu;
            Logger::info("Returned to main menu");
        }
        else if (m_gameState == GameState::MainMenu)
        {
            // Exit from main menu
            requestExit();
        }
    }
}

void
Application::update(float deltaTime)
{
    if (m_gameState == GameState::MainMenu && m_impl->mainMenu)
    {
        m_impl->mainMenu->update(deltaTime);
        if (m_impl->textBox->isVisible())
        {
            m_impl->textBox->update(deltaTime);
        }
    }
    else if (m_gameState == GameState::Playing)
    {
        m_impl->gameLoop->update(deltaTime);
    }
}

void
Application::render(float deltaTime)
{
    (void) deltaTime;  // Unused for now

    if (!m_impl->renderer)
        return;

    m_impl->renderer->beginFrame();

    auto* textRenderer = m_impl->renderer->getTextRenderer();

    if (m_gameState == GameState::MainMenu && m_impl->mainMenu && textRenderer)
    {
        // Render main menu
        m_impl->mainMenu->render(textRenderer);
    }
    else if (m_gameState == GameState::Playing && textRenderer)
    {
        m_impl->gameLoop->render(textRenderer);
    }

    m_impl->textBox->render(textRenderer);

    m_impl->renderer->endFrame();
}

void
Application::syncFrameRate()
{
    if (m_targetFPS <= 0)
    {
        return;  // Unlimited FPS
    }

    auto targetFrameTime = std::chrono::duration<float>(1.0f / static_cast<float>(m_targetFPS));
    auto currentTime     = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> frameTime = currentTime - m_impl->lastFrameTime;

    if (frameTime < targetFrameTime)
    {
        auto sleepTime = targetFrameTime - frameTime;
        std::this_thread::sleep_for(sleepTime);
    }
}

void
Application::setupMainMenu()
{
    if (!m_impl->mainMenu)
        return;

    // Check if save files exist
    bool hasSaves = m_impl->saveSystem && m_impl->saveSystem->hasSaveFiles();
    m_impl->mainMenu->setContinueEnabled(hasSaves);

    // Setup callbacks for each menu option
    m_impl->mainMenu->setCallback(StartMenuOption::NEW_GAME, [this]() {
        Logger::info("New Game selected");
        m_gameState = GameState::Playing;
    });

    m_impl->mainMenu->setCallback(StartMenuOption::CONTINUE, [this]() {
        Logger::info("Continue selected");
        m_gameState = GameState::Playing;
        // TODO: Load last save
    });

    m_impl->mainMenu->setCallback(StartMenuOption::SETTINGS, [this]() {
        Logger::info("Settings selected");
        m_gameState = GameState::Configuration;
        // TODO: Show configuration screen
    });

    m_impl->mainMenu->setCallback(StartMenuOption::CREDITS, [this]() {
        Logger::info("Credits selected");
        // TODO: Show credits screen
    });

    m_impl->mainMenu->setCallback(StartMenuOption::EXIT, [this]() { requestExit(); });

    Logger::info("Start menu setup complete (Continue: {})", hasSaves ? "enabled" : "disabled");
}

void
Application::handleKeyInput(int key, int scancode, int action, int mods)
{
    (void) scancode;  // Unused for now
    (void) mods;      // Unused for now

    if (m_gameState == GameState::MainMenu && m_impl->mainMenu && !m_impl->textBox->isVisible())
    {
        m_impl->mainMenu->handleInput(key, action);
    }
    else if (m_gameState == GameState::MainMenu && m_impl->mainMenu && m_impl->textBox->isVisible())
    {
        m_impl->textBox->handleInput(key, action);
    }
    else if (m_gameState == GameState::Playing && m_impl->gameLoop)
    {
        m_impl->gameLoop->handleInput(key, action);
    }
}

void
Application::handleMouseMove(double x, double y)
{
    (void) x;  // Unused - StartMenu doesn't support mouse
    (void) y;  // Unused - StartMenu doesn't support mouse
    // StartMenu uses keyboard-only navigation
}

void
Application::handleMouseButton(int button, int action, int mods)
{
    (void) button;  // Unused - StartMenu doesn't support mouse
    (void) action;  // Unused - StartMenu doesn't support mouse
    (void) mods;    // Unused - StartMenu doesn't support mouse
    // StartMenu uses keyboard-only navigation
}

void
Application::handleWindowResize()
{
    static int32 lastWidth  = 0;
    static int32 lastHeight = 0;

    int32 currentWidth  = m_impl->window->getWidth();
    int32 currentHeight = m_impl->window->getHeight();

    // Check if window size changed
    if (currentWidth != lastWidth || currentHeight != lastHeight)
    {
        lastWidth  = currentWidth;
        lastHeight = currentHeight;

        Logger::info("Window resized to {}x{}", currentWidth, currentHeight);

        // Update text renderer projection
        if (m_impl->renderer && m_impl->renderer->getTextRenderer())
        {
            m_impl->renderer->getTextRenderer()->updateScreenSize(currentWidth, currentHeight);
        }

        if (m_impl->mainMenu)
        {
            m_impl->mainMenu->onWindowResize(currentWidth, currentHeight);
        }

        if (m_impl->textBox)
        {
            m_impl->textBox->onWindowResize(currentWidth, currentHeight);
        }

        if (m_impl->gameLoop)
        {
            m_impl->gameLoop->onWindowResize(currentWidth, currentHeight);
        }
    }
}
}  // namespace deadcode
