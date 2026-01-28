/**
 * @file ConfigMenu.cpp
 * @brief Implementation of ConfigMenu class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-22
 */

#include "deadcode/ui/ConfigMenu.hpp"

#include "deadcode/core/Config.hpp"
#include "deadcode/core/Logger.hpp"
#include "deadcode/graphics/TextRenderer.hpp"

#include <GLFW/glfw3.h>

namespace deadcode
{

ConfigMenu::ConfigMenu() = default;

ConfigMenu::~ConfigMenu() = default;

bool
ConfigMenu::initialize(int32 screenWidth, int32 screenHeight, Config* config)
{
    Logger::info("Initializing config menu...");

    m_screenWidth  = screenWidth;
    m_screenHeight = screenHeight;
    m_config       = config;

    // Create category tabs frame (top)
    m_categoryFrame = std::make_unique<MenuFrame>(FrameStyle::SINGLE);
    m_categoryFrame->setDimensions(
        static_cast<float32>(screenWidth) / 2.0f - 350.0f,  // Center horizontally
        static_cast<float32>(screenHeight) - 50.0f,         // Near top
        70,                                                 // Width
        5                                                   // Height
    );
    m_categoryFrame->setTitle("SETTINGS", FrameAlign::CENTER);
    m_categoryFrame->setColors(glm::vec3(1.0f, 0.5f, 0.0f),   // Orange border
                               glm::vec3(1.0f, 1.0f, 0.0f),   // Yellow title
                               glm::vec3(1.0f, 1.0f, 1.0f));  // White content
    m_categoryFrame->setPadding(1);
    m_categoryFrame->setScreenDimensions(screenWidth, screenHeight);

    // Create main settings frame (center)
    m_mainFrame = std::make_unique<MenuFrame>(FrameStyle::DOUBLE);
    m_mainFrame->setDimensions(
        static_cast<float32>(screenWidth) / 2.0f - 350.0f,   // Center horizontally
        static_cast<float32>(screenHeight) / 2.0f + 100.0f,  // Below categories
        70,                                                  // Width
        20                                                   // Height
    );
    m_mainFrame->setFooter("LEFT/RIGHT: Category | UP/DOWN: Setting | -/+: Change | ESC: Back",
                           FrameAlign::CENTER);
    m_mainFrame->setColors(glm::vec3(1.0f, 0.5f, 0.0f),   // Orange border
                           glm::vec3(1.0f, 1.0f, 0.0f),   // Yellow title
                           glm::vec3(1.0f, 1.0f, 1.0f));  // White content
    m_mainFrame->setPadding(2);
    m_mainFrame->setScreenDimensions(screenWidth, screenHeight);

    // Initialize settings
    initializeSettings();

    Logger::info("Config menu initialized");
    return true;
}

void
ConfigMenu::initializeSettings()
{
    // DISPLAY settings
    auto& displaySettings = m_settings[static_cast<size_t>(ConfigCategory::DISPLAY)];
    displaySettings.clear();

    ConfigSetting fullscreenSetting;
    fullscreenSetting.name         = "Fullscreen";
    fullscreenSetting.description  = "Toggle fullscreen mode";
    fullscreenSetting.type         = SettingType::TOGGLE;
    fullscreenSetting.currentValue = 0;  // Off
    fullscreenSetting.minValue     = 0;
    fullscreenSetting.maxValue     = 1;
    displaySettings.push_back(fullscreenSetting);

    ConfigSetting vSyncSetting;
    vSyncSetting.name         = "VSync";
    vSyncSetting.description  = "Vertical synchronization";
    vSyncSetting.type         = SettingType::TOGGLE;
    vSyncSetting.currentValue = 1;  // On
    vSyncSetting.minValue     = 0;
    vSyncSetting.maxValue     = 1;
    displaySettings.push_back(vSyncSetting);

    ConfigSetting fpsSetting;
    fpsSetting.name         = "FPS Limit";
    fpsSetting.description  = "Maximum frames per second";
    fpsSetting.type         = SettingType::SLIDER;
    fpsSetting.currentValue = 60;
    fpsSetting.minValue     = 30;
    fpsSetting.maxValue     = 144;
    displaySettings.push_back(fpsSetting);

    ConfigSetting textScaleSetting;
    textScaleSetting.name         = "Text Scale";
    textScaleSetting.description  = "Size of text rendering";
    textScaleSetting.type         = SettingType::SLIDER;
    textScaleSetting.currentValue = 100;  // Percentage
    textScaleSetting.minValue     = 50;
    textScaleSetting.maxValue     = 200;
    displaySettings.push_back(textScaleSetting);

    // AUDIO settings
    auto& audioSettings = m_settings[static_cast<size_t>(ConfigCategory::AUDIO)];
    audioSettings.clear();

    ConfigSetting masterVolumeSetting;
    masterVolumeSetting.name         = "Master Volume";
    masterVolumeSetting.description  = "Overall audio volume";
    masterVolumeSetting.type         = SettingType::SLIDER;
    masterVolumeSetting.currentValue = 80;  // Percentage
    masterVolumeSetting.minValue     = 0;
    masterVolumeSetting.maxValue     = 100;
    audioSettings.push_back(masterVolumeSetting);

    ConfigSetting musicVolumeSetting;
    musicVolumeSetting.name         = "Music Volume";
    musicVolumeSetting.description  = "Background music volume";
    musicVolumeSetting.type         = SettingType::SLIDER;
    musicVolumeSetting.currentValue = 70;
    musicVolumeSetting.minValue     = 0;
    musicVolumeSetting.maxValue     = 100;
    audioSettings.push_back(musicVolumeSetting);

    ConfigSetting sfxVolumeSetting;
    sfxVolumeSetting.name         = "SFX Volume";
    sfxVolumeSetting.description  = "Sound effects volume";
    sfxVolumeSetting.type         = SettingType::SLIDER;
    sfxVolumeSetting.currentValue = 90;
    sfxVolumeSetting.minValue     = 0;
    sfxVolumeSetting.maxValue     = 100;
    audioSettings.push_back(sfxVolumeSetting);

    // GAMEPLAY settings
    auto& gameplaySettings = m_settings[static_cast<size_t>(ConfigCategory::GAMEPLAY)];
    gameplaySettings.clear();

    ConfigSetting difficultySetting;
    difficultySetting.name         = "Difficulty";
    difficultySetting.description  = "Game challenge level";
    difficultySetting.type         = SettingType::CHOICE;
    difficultySetting.currentValue = 1;  // Normal
    difficultySetting.minValue     = 0;
    difficultySetting.maxValue     = 2;
    difficultySetting.choices      = {"Easy", "Normal", "Hard"};
    gameplaySettings.push_back(difficultySetting);

    ConfigSetting autosaveSetting;
    autosaveSetting.name         = "Autosave";
    autosaveSetting.description  = "Automatic save game";
    autosaveSetting.type         = SettingType::TOGGLE;
    autosaveSetting.currentValue = 1;  // On
    autosaveSetting.minValue     = 0;
    autosaveSetting.maxValue     = 1;
    gameplaySettings.push_back(autosaveSetting);

    ConfigSetting textSpeedSetting;
    textSpeedSetting.name         = "Text Speed";
    textSpeedSetting.description  = "Dialogue text display speed";
    textSpeedSetting.type         = SettingType::SLIDER;
    textSpeedSetting.currentValue = 50;
    textSpeedSetting.minValue     = 10;
    textSpeedSetting.maxValue     = 100;
    gameplaySettings.push_back(textSpeedSetting);

    // CONTROLS settings
    auto& controlSettings = m_settings[static_cast<size_t>(ConfigCategory::CONTROLS)];
    controlSettings.clear();

    ConfigSetting mouseSetting;
    mouseSetting.name         = "Mouse Support";
    mouseSetting.description  = "Enable mouse controls";
    mouseSetting.type         = SettingType::TOGGLE;
    mouseSetting.currentValue = 1;  // On
    mouseSetting.minValue     = 0;
    mouseSetting.maxValue     = 1;
    controlSettings.push_back(mouseSetting);

    ConfigSetting keyboardLayoutSetting;
    keyboardLayoutSetting.name         = "Keyboard Layout";
    keyboardLayoutSetting.description  = "Key binding preset";
    keyboardLayoutSetting.type         = SettingType::CHOICE;
    keyboardLayoutSetting.currentValue = 0;  // QWERTY
    keyboardLayoutSetting.minValue     = 0;
    keyboardLayoutSetting.maxValue     = 1;
    keyboardLayoutSetting.choices      = {"QWERTY", "AZERTY"};
    controlSettings.push_back(keyboardLayoutSetting);
}

void
ConfigMenu::update(float deltaTime)
{
    if (!m_visible)
        return;

    // Update blink animation
    m_blinkTimer += deltaTime;
    if (m_blinkTimer >= 0.5f)
    {
        m_blinkState = !m_blinkState;
        m_blinkTimer = 0.0f;
    }
}

void
ConfigMenu::render(TextRenderer* textRenderer)
{
    if (!m_visible || !textRenderer)
        return;

    // Render category tabs
    renderCategories(textRenderer);

    // Render settings for current category
    renderSettings(textRenderer);
}

void
ConfigMenu::renderCategories(TextRenderer* textRenderer)
{
    if (!m_categoryFrame || !textRenderer)
        return;

    m_categoryFrame->render(textRenderer, 0.6f);

    // Build category tabs
    String categoryLine;
    for (int32 i = 0; i < static_cast<int32>(ConfigCategory::COUNT); ++i)
    {
        ConfigCategory cat  = static_cast<ConfigCategory>(i);
        bool isSelected     = (cat == m_selectedCategory);
        String categoryName = getCategoryName(cat);

        if (isSelected)
        {
            categoryLine += "[ " + categoryName + " ] ";
        }
        else
        {
            categoryLine += "  " + categoryName + "  ";
        }
    }

    m_categoryFrame->renderText(textRenderer, categoryLine, 1, FrameAlign::CENTER, 0.6f);
}

void
ConfigMenu::renderSettings(TextRenderer* textRenderer)
{
    if (!m_mainFrame || !textRenderer)
        return;

    // Update main frame title with current category
    m_mainFrame->setTitle(getCategoryName(m_selectedCategory), FrameAlign::CENTER);
    m_mainFrame->render(textRenderer, 0.6f);

    // Get settings for current category
    const auto& settings = m_settings[static_cast<size_t>(m_selectedCategory)];

    if (settings.empty())
    {
        m_mainFrame->renderText(textRenderer, "No settings available", 2, FrameAlign::CENTER, 0.6f);
        return;
    }

    // Render each setting
    int32 offsetY = 1;
    for (size_t i = 0; i < settings.size(); ++i)
    {
        const auto& setting = settings[i];
        bool isSelected     = (static_cast<int32>(i) == m_selectedSetting);

        // Build setting line
        String prefix      = isSelected && m_blinkState ? "> " : "  ";
        String settingLine = prefix + setting.name + ": " + renderSettingValue(setting);

        m_mainFrame->renderText(textRenderer, settingLine, offsetY, FrameAlign::LEFT, 0.6f);

        // Render description for selected item
        if (isSelected)
        {
            String descLine = "    " + setting.description;
            m_mainFrame->renderText(textRenderer, descLine, offsetY + 1, FrameAlign::LEFT, 0.5f);
            offsetY += 3;  // Extra space for description
        }
        else
        {
            offsetY += 2;  // Normal spacing
        }
    }
}

String
ConfigMenu::renderSettingValue(const ConfigSetting& setting) const
{
    switch (setting.type)
    {
        case SettingType::TOGGLE:
            return setting.currentValue == 1 ? "[ON]" : "[OFF]";

        case SettingType::SLIDER:
            return "[" + std::to_string(setting.currentValue) + "]";

        case SettingType::CHOICE:
            if (setting.currentValue >= 0 &&
                setting.currentValue < static_cast<int32>(setting.choices.size()))
            {
                return "[" + setting.choices[setting.currentValue] + "]";
            }
            return "[Unknown]";

        case SettingType::KEY_BIND:
            return "[Press Key]";

        default:
            return "[?]";
    }
}

void
ConfigMenu::handleInput(int32 key, int32 action)
{
    if (!m_visible || action != GLFW_PRESS)
        return;

    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            if (m_onClose)
                m_onClose();
            break;

        case GLFW_KEY_LEFT:
        case GLFW_KEY_A:
            previousCategory();
            break;

        case GLFW_KEY_RIGHT:
        case GLFW_KEY_D:
            nextCategory();
            break;

        case GLFW_KEY_UP:
        case GLFW_KEY_W:
            moveSelectionUp();
            break;

        case GLFW_KEY_DOWN:
        case GLFW_KEY_S:
            moveSelectionDown();
            break;

        case GLFW_KEY_MINUS:
        case GLFW_KEY_KP_SUBTRACT:
            decreaseSetting();
            break;

        case GLFW_KEY_EQUAL:  // '+' key (with shift)
        case GLFW_KEY_KP_ADD:
            increaseSetting();
            break;

        default:
            break;
    }
}

void
ConfigMenu::previousCategory()
{
    int32 current = static_cast<int32>(m_selectedCategory);
    current--;
    if (current < 0)
    {
        current = static_cast<int32>(ConfigCategory::COUNT) - 1;
    }
    m_selectedCategory = static_cast<ConfigCategory>(current);
    m_selectedSetting  = 0;  // Reset to first setting in category

    Logger::debug("Config category: {}", getCategoryName(m_selectedCategory));
}

void
ConfigMenu::nextCategory()
{
    int32 current = static_cast<int32>(m_selectedCategory);
    current++;
    if (current >= static_cast<int32>(ConfigCategory::COUNT))
    {
        current = 0;
    }
    m_selectedCategory = static_cast<ConfigCategory>(current);
    m_selectedSetting  = 0;  // Reset to first setting in category

    Logger::debug("Config category: {}", getCategoryName(m_selectedCategory));
}

void
ConfigMenu::moveSelectionUp()
{
    const auto& settings = m_settings[static_cast<size_t>(m_selectedCategory)];
    if (settings.empty())
        return;

    m_selectedSetting--;
    if (m_selectedSetting < 0)
    {
        m_selectedSetting = static_cast<int32>(settings.size()) - 1;
    }

    m_blinkTimer = 0.0f;
    m_blinkState = true;
}

void
ConfigMenu::moveSelectionDown()
{
    const auto& settings = m_settings[static_cast<size_t>(m_selectedCategory)];
    if (settings.empty())
        return;

    m_selectedSetting++;
    if (m_selectedSetting >= static_cast<int32>(settings.size()))
    {
        m_selectedSetting = 0;
    }

    m_blinkTimer = 0.0f;
    m_blinkState = true;
}

void
ConfigMenu::decreaseSetting()
{
    auto& settings = m_settings[static_cast<size_t>(m_selectedCategory)];
    if (m_selectedSetting < 0 || m_selectedSetting >= static_cast<int32>(settings.size()))
        return;

    auto& setting = settings[static_cast<size_t>(m_selectedSetting)];

    if (setting.type == SettingType::SLIDER || setting.type == SettingType::CHOICE)
    {
        setting.currentValue--;
        if (setting.currentValue < setting.minValue)
        {
            setting.currentValue = setting.minValue;
        }
    }
    else if (setting.type == SettingType::TOGGLE)
    {
        setting.currentValue = 0;  // Off
    }

    Logger::debug("Setting {} = {}", setting.name, setting.currentValue);
}

void
ConfigMenu::increaseSetting()
{
    auto& settings = m_settings[static_cast<size_t>(m_selectedCategory)];
    if (m_selectedSetting < 0 || m_selectedSetting >= static_cast<int32>(settings.size()))
        return;

    auto& setting = settings[static_cast<size_t>(m_selectedSetting)];

    if (setting.type == SettingType::SLIDER || setting.type == SettingType::CHOICE)
    {
        setting.currentValue++;
        if (setting.currentValue > setting.maxValue)
        {
            setting.currentValue = setting.maxValue;
        }
    }
    else if (setting.type == SettingType::TOGGLE)
    {
        setting.currentValue = 1;  // On
    }

    Logger::debug("Setting {} = {}", setting.name, setting.currentValue);
}

String
ConfigMenu::getCategoryName(ConfigCategory category) const
{
    switch (category)
    {
        case ConfigCategory::DISPLAY:
            return "DISPLAY";
        case ConfigCategory::AUDIO:
            return "AUDIO";
        case ConfigCategory::GAMEPLAY:
            return "GAMEPLAY";
        case ConfigCategory::CONTROLS:
            return "CONTROLS";
        default:
            return "UNKNOWN";
    }
}

void
ConfigMenu::applySettings()
{
    Logger::info("Applying settings...");
    // TODO: Apply settings to config and save
}

void
ConfigMenu::revertSettings()
{
    Logger::info("Reverting settings...");
    // TODO: Reload settings from config
    initializeSettings();
}

}  // namespace deadcode
