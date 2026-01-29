/**
 * @file StartMenu.cpp
 * @brief Implementation of StartMenu class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-22
 */

#include "deadcode/ui/StartMenu.hpp"

#include "deadcode/core/Logger.hpp"
#include "deadcode/core/Version.hpp"
#include "deadcode/graphics/TextRenderer.hpp"

#include <GLFW/glfw3.h>

namespace deadcode
{

StartMenu::StartMenu()
{
    // Initialize ASCII art logo for 0xDEADC0DE
    m_logoLines = {"   ___        ____  _____    _    ____   ____ ___  ____  _____ ",
                   "  / _ \\ __  _|  _ \\| ____|  / \\  |  _ \\ / ___/ _ \\|  _ \\| ____|",
                   " | | | |\\ \\/ / | | |  _|   / _ \\ | | | | |  | | | | | | |  _|  ",
                   " | |_| | >  <| |_| | |___ / ___ \\| |_| | |__| |_| | |_| | |___ ",
                   "  \\___/ /_/\\_\\____/|_____/_/   \\_\\____/ \\____\\___/|____/|_____|",
                   "",
                   "              T E X T - B A S E D   R P G                       "};
}

StartMenu::~StartMenu() = default;

bool
StartMenu::initialize(int32 screenWidth, int32 screenHeight)
{
    Logger::info("Initializing start menu...");

    m_screenWidth  = screenWidth;
    m_screenHeight = screenHeight;

    // Create logo frame (top section)
    m_logoFrame = std::make_unique<MenuFrame>(FrameStyle::DOUBLE);
    m_logoFrame->setDimensions(
        static_cast<float32>(screenWidth) / 2.0f - 500.0f,  // Center horizontally
        static_cast<float32>(screenHeight) - 50.0f,         // Near top
        70,                                                 // Width in chars
        10                                                  // Height in chars
    );
    m_logoFrame->setTitle("WELCOME TO", FrameAlign::CENTER);
    m_logoFrame->setColors(glm::vec3(0.0f, 1.0f, 1.0f),   // Cyan border
                           glm::vec3(1.0f, 1.0f, 0.0f),   // Yellow title
                           glm::vec3(0.0f, 1.0f, 0.0f));  // Green content
    m_logoFrame->setPadding(1);
    m_logoFrame->setScreenDimensions(screenWidth, screenHeight);
    float32 logoScale = m_logoFrame->calculateDynamicScale(screenWidth, screenHeight, 0.4f);
    m_logoFrame->setScale(logoScale);

    // Create main menu frame (center section)
    m_mainFrame = std::make_unique<MenuFrame>(FrameStyle::DOUBLE);
    m_mainFrame->setDimensions(
        static_cast<float32>(screenWidth) / 2.0f - 200.0f,  // Center horizontally
        static_cast<float32>(screenHeight) / 2.0f + 50.0f,  // Center vertically
        40,                                                 // Width in chars
        12                                                  // Height in chars
    );
    m_mainFrame->setTitle("MAIN MENU", FrameAlign::CENTER);
    // m_mainFrame->setFooter("Use ARROW KEYS to navigate, ENTER to select", FrameAlign::CENTER);
    m_mainFrame->setColors(glm::vec3(0.0f, 1.0f, 1.0f),   // Cyan border
                           glm::vec3(1.0f, 1.0f, 0.0f),   // Yellow title
                           glm::vec3(1.0f, 1.0f, 1.0f));  // White content
    m_mainFrame->setPadding(2);
    m_mainFrame->setScreenDimensions(screenWidth, screenHeight);

    float32 menuScale = m_mainFrame->calculateDynamicScale(screenWidth, screenHeight, 1.0f);
    m_mainFrame->setScale(menuScale);

    Logger::info("Start menu initialized");
    return true;
}

void
StartMenu::update(float deltaTime)
{
    if (!m_visible)
        return;

    // Update blink animation for selected item
    m_blinkTimer += deltaTime;
    if (m_blinkTimer >= 0.5f)  // Blink every 0.5 seconds
    {
        m_blinkState = !m_blinkState;
        m_blinkTimer = 0.0f;
    }

    // Update glitch effect timer
    m_glitchTimer += deltaTime;
    if (m_glitchTimer >= 3.0f)  // Reset every 3 seconds
    {
        m_glitchTimer = 0.0f;
    }
}

void
StartMenu::render(TextRenderer* textRenderer)
{
    if (!m_visible || !textRenderer)
        return;

    // Render logo frame
    renderLogo(textRenderer);

    // Render main menu frame
    renderOptions(textRenderer);

    // Render version/footer info
    renderFooter(textRenderer);
}

void
StartMenu::renderLogo(TextRenderer* textRenderer)
{
    if (!m_logoFrame || !textRenderer)
        return;

    // Render frame
    float32 scale = m_logoFrame->m_scale;
    m_logoFrame->render(textRenderer, scale);

    // Render ASCII art logo
    float32 contentX, contentY;
    int32 contentWidth, contentHeight;
    m_logoFrame->getContentArea(textRenderer, scale, contentX, contentY, contentWidth,
                                contentHeight);

    glm::vec3 logoColor = glm::vec3(0.0f, 1.0f, 0.0f);  // Green

    // Add glitch effect randomly
    if (m_glitchTimer > 2.8f)  // Glitch near end of cycle
    {
        logoColor = glm::vec3(1.0f, 0.0f, 0.0f);  // Red glitch
    }

    float32 lineHeight    = textRenderer->getLineHeight(scale);
    float32 screenCenterX = static_cast<float32>(m_screenWidth) / 2.0f;

    int32 lineOffset = 0;
    for (const auto& line : m_logoLines)
    {
        float32 lineWidth = textRenderer->getTextWidth(line, scale);
        float32 centerX   = screenCenterX - (lineWidth) / 2.0f;

        textRenderer->renderText(line, centerX, contentY - lineOffset * lineHeight, scale,
                                 logoColor);

        lineOffset += 1;
    }
}

void
StartMenu::renderOptions(TextRenderer* textRenderer)
{
    if (!m_mainFrame || !textRenderer)
        return;

    float32 scale = m_mainFrame->m_scale;

    // Render frame
    m_mainFrame->render(textRenderer, scale);

    float32 contentX, contentY;
    int32 contentWidth, contentHeight;

    m_mainFrame->getContentArea(textRenderer, scale, contentX, contentY, contentWidth,
                                contentHeight);

    glm::vec3 optionColor = glm::vec3(1.0f, 1.0f, 1.0f);

    float32 lineHeight    = textRenderer->getLineHeight(scale);
    float32 screenCenterX = static_cast<float32>(m_screenWidth) / 2.0f;

    int32 lineOffset = 0;

    for (int32 i = 0; i < static_cast<int32>(StartMenuOption::COUNT); ++i)
    {
        StartMenuOption option = static_cast<StartMenuOption>(i);
        String optionText      = getOptionText(option);

        bool isSelected = (option == m_selectedOption);

        bool isEnabled = isOptionEnabled(option);

        String prefix = "  ";
        if (isSelected && m_blinkState)
        {
            prefix = "> ";
        }
        else if (isSelected)
        {
            prefix = "  ";
        }

        if (!isEnabled)
        {
            optionText = "[" + optionText + "]";
        }

        String fullLine     = prefix + optionText;
        float32 prefixWidth = textRenderer->getTextWidth(prefix, scale);
        float32 lineWidth   = textRenderer->getTextWidth(fullLine, scale);
        float32 centerX     = screenCenterX - ((lineWidth + prefixWidth) / 2.0f);

        Logger::info("CENTER X: {}", centerX);

        textRenderer->renderText(fullLine, centerX, contentY - (lineOffset * lineHeight * 2.0f),
                                 scale, optionColor);

        lineOffset++;
    }
}

void
StartMenu::renderFooter(TextRenderer* textRenderer)
{
    if (!textRenderer)
        return;

    constexpr float32 FOOTER_SCALE   = 0.4f;
    constexpr glm::vec3 FOOTER_COLOR = glm::vec3(0.5f, 0.5f, 0.5f);

    // Render version info at bottom
    String versionText = "Version " + Version::getVersionString();
    float32 versionX   = static_cast<float32>(m_screenWidth) -
                       textRenderer->getTextWidth(versionText, 0.4f) - 20.0f;
    float32 versionY = 20.0f;

    textRenderer->renderText(versionText, versionX, versionY, FOOTER_SCALE,
                             FOOTER_COLOR);  // Gray

    // Render copyright
    String copyrightText = "(c) 2026 0xDEADC0DE Team";
    float32 copyrightX   = 20.0f;
    float32 copyrightY   = 20.0f;

    textRenderer->renderText(copyrightText, copyrightX, copyrightY, FOOTER_SCALE,
                             FOOTER_COLOR);  // Gray
}

void
StartMenu::handleInput(int32 key, int32 action)
{
    if (!m_visible || action != GLFW_PRESS)
        return;

    switch (key)
    {
        case GLFW_KEY_UP:
        case GLFW_KEY_W:
            moveSelectionUp();
            break;

        case GLFW_KEY_DOWN:
        case GLFW_KEY_S:
            moveSelectionDown();
            break;

        case GLFW_KEY_ENTER:
        case GLFW_KEY_SPACE:
            executeSelection();
            break;

        case GLFW_KEY_ESCAPE:
            // ESC goes to exit
            m_selectedOption = StartMenuOption::EXIT;
            break;

        default:
            break;
    }
}

void
StartMenu::setCallback(StartMenuOption option, std::function<void()> callback)
{
    size_t index = static_cast<size_t>(option);
    if (index < static_cast<size_t>(StartMenuOption::COUNT))
    {
        m_callbacks[index] = callback;
    }
}

void
StartMenu::setContinueEnabled(bool enabled)
{
    m_continueEnabled = enabled;
}

String
StartMenu::getOptionText(StartMenuOption option) const
{
    switch (option)
    {
        case StartMenuOption::NEW_GAME:
            return "New Game";
        case StartMenuOption::CONTINUE:
            return "Continue";
        case StartMenuOption::SETTINGS:
            return "Settings";
        case StartMenuOption::CREDITS:
            return "Credits";
        case StartMenuOption::EXIT:
            return "Exit";
        default:
            return "Unknown";
    }
}

bool
StartMenu::isOptionEnabled(StartMenuOption option) const
{
    if (option == StartMenuOption::CONTINUE)
    {
        return m_continueEnabled;
    }
    return true;
}

void
StartMenu::moveSelectionUp()
{
    int32 current = static_cast<int32>(m_selectedOption);
    do
    {
        current--;
        if (current < 0)
        {
            current = static_cast<int32>(StartMenuOption::COUNT) - 1;
        }
        m_selectedOption = static_cast<StartMenuOption>(current);
    }
    while (!isOptionEnabled(m_selectedOption));

    m_blinkTimer = 0.0f;
    m_blinkState = true;

    Logger::debug("Menu selection: {}", getOptionText(m_selectedOption));
}

void
StartMenu::moveSelectionDown()
{
    int32 current = static_cast<int32>(m_selectedOption);
    do
    {
        current++;
        if (current >= static_cast<int32>(StartMenuOption::COUNT))
        {
            current = 0;
        }
        m_selectedOption = static_cast<StartMenuOption>(current);
    }
    while (!isOptionEnabled(m_selectedOption));

    m_blinkTimer = 0.0f;
    m_blinkState = true;

    Logger::debug("Menu selection: {}", getOptionText(m_selectedOption));
}

void
StartMenu::executeSelection()
{
    if (!isOptionEnabled(m_selectedOption))
        return;

    size_t index = static_cast<size_t>(m_selectedOption);
    if (m_callbacks[index])
    {
        Logger::info("Executing menu option: {}", getOptionText(m_selectedOption));
        m_callbacks[index]();
    }
}

void
StartMenu::onWindowResize(int32 screenWidth, int32 screenHeight)
{
    Logger::info("StartMenu handling window resize: {}x{}", screenWidth, screenHeight);

    m_screenWidth  = screenWidth;
    m_screenHeight = screenHeight;

    if (m_logoFrame)
    {
        m_logoFrame->setScreenDimensions(screenWidth, screenHeight);
        float32 logoScale = m_logoFrame->calculateDynamicScale(screenWidth, screenHeight, 0.4f);
        m_logoFrame->setScale(logoScale);

        int32 maxLogoLength = 0;
        for (const auto& line : m_logoLines)
        {
            if (static_cast<int32>(line.length()) > maxLogoLength)
            {
                maxLogoLength = static_cast<int32>(line.length());
            }
        }

        int32 frameWidth  = maxLogoLength + 4;
        int32 frameHeight = static_cast<int32>(m_logoLines.size()) + 4;

        m_logoFrame->setDimensions(static_cast<float32>(screenWidth) / 2.0f - (frameWidth * 0.8f),
                                   static_cast<float32>(screenHeight) - 50.0f, frameWidth,
                                   frameHeight);

        Logger::debug("Logo frame scale updated to: {}", logoScale);
    }

    if (m_mainFrame)
    {
        m_mainFrame->setScreenDimensions(screenWidth, screenHeight);
        float32 menuScale = m_mainFrame->calculateDynamicScale(screenWidth, screenHeight, 1.0f);
        m_mainFrame->setScale(menuScale);
        constexpr float32 MENU_BOTTOM_OFFSET = 100.0f;
        float32 menuY = static_cast<float32>(screenHeight) - MENU_BOTTOM_OFFSET;

        constexpr int32 CHAR_HEIGHT_PIXELS   = 32;
        constexpr float32 LOGO_BOTTOM_OFFSET = 300.0f;

        int32 availablePixels = static_cast<int32>(menuY - LOGO_BOTTOM_OFFSET);
        int32 availableHeight = std::max(1, availablePixels / CHAR_HEIGHT_PIXELS);

        int32 optionCount    = static_cast<int32>(StartMenuOption::COUNT);
        int32 requiredHeight = (optionCount * 2) + 4;

        int32 frameHeight = std::max(10, std::min(requiredHeight, availableHeight));

        m_mainFrame->setDimensions(static_cast<float32>(screenWidth) / 2.0f - 200.0f, menuY, 40,
                                   frameHeight);

        Logger::debug("Main frame scale: {}, height: {}, position: {}, available: {}px", menuScale,
                      frameHeight, static_cast<int32>(menuY), availablePixels);
    }
}

}  // namespace deadcode
