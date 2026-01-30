/**
 * @file StartMenu.hpp
 * @brief Main start menu for the game
 *
 * Beautiful main menu with ASCII art logo, menu options,
 * and visual effects. This is the first screen players see.
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-22
 */

#pragma once

#include "deadcode/core/Types.hpp"
#include "deadcode/graphics/GlitchEffect.hpp"
#include "deadcode/ui/MenuFrame.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace deadcode
{

class TextRenderer;
class InputManager;

/**
 * @brief Menu options in start menu
 */
enum class StartMenuOption
{
    NEW_GAME,
    CONTINUE,
    SETTINGS,
    CREDITS,
    EXIT,
    COUNT
};

/**
 * @brief Main start menu
 *
 * Displays game logo, menu options, and handles user selection.
 * Uses beautiful ASCII art frames and animations.
 */
class StartMenu
{
public:
    /**
     * @brief Constructor
     */
    StartMenu();

    /**
     * @brief Destructor
     */
    ~StartMenu();

    /**
     * @brief Initialize start menu
     *
     * @param screenWidth Screen width in pixels
     * @param screenHeight Screen height in pixels
     * @return true if successful
     */
    bool initialize(int32 screenWidth, int32 screenHeight);

    /**
     * @brief Update menu state
     *
     * @param deltaTime Time since last frame
     */
    void update(float deltaTime);

    /**
     * @brief Render the menu
     *
     * @param textRenderer TextRenderer to use for drawing
     */
    void render(TextRenderer* textRenderer);

    /**
     * @brief Handle keyboard input
     *
     * @param key GLFW key code
     * @param action GLFW action (press/release)
     */
    void handleInput(int32 key, int32 action);

    /**
     * @brief Set callback for menu option selection
     *
     * @param option Menu option
     * @param callback Function to call when selected
     */
    void setCallback(StartMenuOption option, std::function<void()> callback);

    /**
     * @brief Check if Continue option should be enabled
     *
     * @param enabled Whether continue is available
     */
    void setContinueEnabled(bool enabled);

    /**
     * @brief Get currently selected option
     *
     * @return Current selection
     */
    StartMenuOption
    getSelectedOption() const
    {
        return m_selectedOption;
    }

    /**
     * @brief Show/hide the menu
     *
     * @param visible Whether menu should be visible
     */
    void
    setVisible(bool visible)
    {
        m_visible = visible;
    }

    /**
     * @brief Check if menu is visible
     */
    bool
    isVisible() const
    {
        return m_visible;
    }

    void onWindowResize(int32 screenWidth, int32 screenHeight);

private:
    /**
     * @brief Render game logo
     */
    void renderLogo(TextRenderer* textRenderer);

    /**
     * @brief Render menu options
     */
    void renderOptions(TextRenderer* textRenderer);

    /**
     * @brief Render version info and credits
     */
    void renderFooter(TextRenderer* textRenderer);

    /**
     * @brief Get option text
     */
    String getOptionText(StartMenuOption option) const;

    /**
     * @brief Check if option is enabled
     */
    bool isOptionEnabled(StartMenuOption option) const;

    /**
     * @brief Move selection up
     */
    void moveSelectionUp();

    /**
     * @brief Move selection down
     */
    void moveSelectionDown();

    /**
     * @brief Execute current selection
     */
    void executeSelection();

    int32 m_screenWidth{800};
    int32 m_screenHeight{600};
    bool m_visible{true};
    bool m_continueEnabled{false};

    StartMenuOption m_selectedOption{StartMenuOption::NEW_GAME};
    std::function<void()> m_callbacks[static_cast<size_t>(StartMenuOption::COUNT)];

    std::unique_ptr<MenuFrame> m_mainFrame;
    std::unique_ptr<MenuFrame> m_logoFrame;

    // Animation state
    float32 m_blinkTimer{0.0f};
    bool m_blinkState{true};
    float32 m_glitchTimer{0.0f};

    // Glitch effect
    std::unique_ptr<GlitchEffect> m_glitchEffect;

    // ASCII art logo
    std::vector<String> m_logoLines;
};

}  // namespace deadcode
