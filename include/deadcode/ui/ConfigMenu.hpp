/**
 * @file ConfigMenu.hpp
 * @brief Configuration/Settings menu
 *
 * Allows players to adjust game settings like display,
 * audio, controls, and gameplay options.
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-22
 */

#pragma once

#include "deadcode/core/Types.hpp"
#include "deadcode/ui/MenuFrame.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace deadcode
{

class TextRenderer;
class Config;

/**
 * @brief Configuration categories
 */
enum class ConfigCategory
{
    DISPLAY,
    AUDIO,
    GAMEPLAY,
    CONTROLS,
    COUNT
};

/**
 * @brief Configuration setting types
 */
enum class SettingType
{
    TOGGLE,   ///< On/Off boolean
    SLIDER,   ///< Numeric range
    CHOICE,   ///< Multiple options
    KEY_BIND  ///< Key binding
};

/**
 * @brief Single configuration setting
 */
struct ConfigSetting
{
    String name;
    String description;
    SettingType type;
    int32 currentValue;
    int32 minValue;
    int32 maxValue;
    std::vector<String> choices;
    std::function<void(int32)> onChange;
};

/**
 * @brief Configuration/Settings menu
 *
 * Displays game settings organized by category with
 * beautiful frames and interactive controls.
 */
class ConfigMenu
{
public:
    /**
     * @brief Constructor
     */
    ConfigMenu();

    /**
     * @brief Destructor
     */
    ~ConfigMenu();

    /**
     * @brief Initialize config menu
     *
     * @param screenWidth Screen width in pixels
     * @param screenHeight Screen height in pixels
     * @param config Configuration object to modify
     * @return true if successful
     */
    bool initialize(int32 screenWidth, int32 screenHeight, Config* config);

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

    /**
     * @brief Set callback for when menu is closed
     *
     * @param callback Function to call
     */
    void
    setOnCloseCallback(std::function<void()> callback)
    {
        m_onClose = callback;
    }

    /**
     * @brief Apply and save all settings
     */
    void applySettings();

    /**
     * @brief Revert changes without saving
     */
    void revertSettings();

private:
    /**
     * @brief Initialize settings for each category
     */
    void initializeSettings();

    /**
     * @brief Render category tabs
     */
    void renderCategories(TextRenderer* textRenderer);

    /**
     * @brief Render settings for current category
     */
    void renderSettings(TextRenderer* textRenderer);

    /**
     * @brief Render setting value
     */
    String renderSettingValue(const ConfigSetting& setting) const;

    /**
     * @brief Move to previous category
     */
    void previousCategory();

    /**
     * @brief Move to next category
     */
    void nextCategory();

    /**
     * @brief Move selection up
     */
    void moveSelectionUp();

    /**
     * @brief Move selection down
     */
    void moveSelectionDown();

    /**
     * @brief Modify current setting (decrease)
     */
    void decreaseSetting();

    /**
     * @brief Modify current setting (increase)
     */
    void increaseSetting();

    /**
     * @brief Get category name
     */
    String getCategoryName(ConfigCategory category) const;

    int32 m_screenWidth{800};
    int32 m_screenHeight{600};
    bool m_visible{false};

    ConfigCategory m_selectedCategory{ConfigCategory::DISPLAY};
    int32 m_selectedSetting{0};

    Config* m_config{nullptr};
    std::vector<ConfigSetting> m_settings[static_cast<size_t>(ConfigCategory::COUNT)];

    std::unique_ptr<MenuFrame> m_mainFrame;
    std::unique_ptr<MenuFrame> m_categoryFrame;

    std::function<void()> m_onClose;

    // Animation
    float32 m_blinkTimer{0.0f};
    bool m_blinkState{true};
};

}  // namespace deadcode
