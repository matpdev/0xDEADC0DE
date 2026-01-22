/**
 * @file Menu.cpp
 * @brief Implementation of the Menu class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-22
 */

#include "deadcode/ui/Menu.hpp"

#include "deadcode/core/Logger.hpp"
#include "deadcode/graphics/TextRenderer.hpp"

#include <GLFW/glfw3.h>

namespace deadcode
{

Menu::Menu() = default;

Menu::~Menu() = default;

bool
Menu::initialize(int32 screenWidth, int32 screenHeight)
{
    Logger::info("Initializing menu system...");

    m_screenWidth  = screenWidth;
    m_screenHeight = screenHeight;

    calculatePositions();

    Logger::info("Menu system initialized");
    return true;
}

void
Menu::addItem(const String& label, std::function<void()> callback, bool enabled)
{
    MenuItem item;
    item.label    = label;
    item.callback = callback;
    item.enabled  = enabled;

    m_items.push_back(item);
    calculatePositions();
}

void
Menu::clear()
{
    m_items.clear();
    m_selectedIndex = 0;
    m_itemPositions.clear();
}

void
Menu::update(float deltaTime)
{
    (void) deltaTime;  // Unused for now
    // Menu update logic if needed
}

void
Menu::render(TextRenderer* textRenderer)
{
    if (!textRenderer || m_items.empty())
        return;

    // Store TextRenderer for width calculations
    m_textRenderer = textRenderer;

    // Render title if set
    if (!m_title.empty())
    {
        float32 titleScale = m_titleFontSize / 48.0f;
        float32 titleWidth = textRenderer->getTextWidth(m_title, titleScale);
        float32 titleX     = (static_cast<float32>(m_screenWidth) - titleWidth) / 2.0f;
        float32 titleY     = static_cast<float32>(m_screenHeight) * 0.8f;  // 80% from bottom

        glm::vec3 titleColor(0.0f, 1.0f, 0.0f);  // Green
        textRenderer->renderText(m_title, titleX, titleY, titleScale, titleColor);
    }

    // Render version if set
    if (!m_version.empty())
    {
        float32 versionScale = m_versionFontSize / 48.0f;
        float32 versionWidth = textRenderer->getTextWidth(m_version, versionScale);
        float32 versionX     = (static_cast<float32>(m_screenWidth) - versionWidth) / 2.0f;
        float32 versionY     = static_cast<float32>(m_screenHeight) * 0.72f;  // Below title

        glm::vec3 versionColor(0.7f, 0.7f, 0.7f);  // Light gray
        textRenderer->renderText(m_version, versionX, versionY, versionScale, versionColor);
    }

    // Render menu items
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        const auto& item = m_items[i];
        const auto& pos  = m_itemPositions[i];

        // Choose color based on state
        glm::vec3 color;
        if (!item.enabled)
        {
            color = glm::vec3(0.3f, 0.3f, 0.3f);  // Gray for disabled
        }
        else if (static_cast<int32>(i) == m_selectedIndex)
        {
            color = glm::vec3(0.0f, 1.0f, 0.0f);  // Green for selected
        }
        else
        {
            color = glm::vec3(1.0f, 1.0f, 1.0f);  // White for normal
        }

        // Add selection indicator and recalculate center position
        String displayText = item.label;
        if (static_cast<int32>(i) == m_selectedIndex && item.enabled)
        {
            displayText = "> " + displayText + " <";
        }

        // Calculate actual text width using TextRenderer
        float32 displayWidth = textRenderer->getTextWidth(displayText, 1.0f);
        float32 renderX      = (static_cast<float32>(m_screenWidth) - displayWidth) / 2.0f;

        textRenderer->renderText(displayText, renderX, pos.y, 1.0f, color);
    }
}

void
Menu::handleKeyboard(int key, int action)
{
    if (action != GLFW_PRESS && action != GLFW_REPEAT)
        return;

    if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
    {
        // Move selection up
        do
        {
            m_selectedIndex--;
            if (m_selectedIndex < 0)
                m_selectedIndex = static_cast<int32>(m_items.size()) - 1;
        }
        while (!m_items[static_cast<size_t>(m_selectedIndex)].enabled &&
               m_selectedIndex >= 0);  // Skip disabled items
    }
    else if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
    {
        // Move selection down
        do
        {
            m_selectedIndex++;
            if (m_selectedIndex >= static_cast<int32>(m_items.size()))
                m_selectedIndex = 0;
        }
        while (!m_items[static_cast<size_t>(m_selectedIndex)].enabled &&
               m_selectedIndex < static_cast<int32>(m_items.size()));  // Skip disabled items
    }
    else if (key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE)
    {
        // Activate selected item
        if (m_selectedIndex >= 0 && m_selectedIndex < static_cast<int32>(m_items.size()))
        {
            const auto& item = m_items[static_cast<size_t>(m_selectedIndex)];
            if (item.enabled && item.callback)
            {
                Logger::info("Menu item selected: {}", item.label);
                item.callback();
            }
        }
    }
}

void
Menu::handleMouseMove(float32 x, float32 y)
{
    // Check which item the mouse is over
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i].enabled && isMouseOverItem(static_cast<int32>(i), x, y))
        {
            m_selectedIndex = static_cast<int32>(i);
            break;
        }
    }
}

void
Menu::handleMouseClick(int button, int action, float32 x, float32 y)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // Check if clicking on an item
        for (size_t i = 0; i < m_items.size(); ++i)
        {
            if (m_items[i].enabled && isMouseOverItem(static_cast<int32>(i), x, y))
            {
                const auto& item = m_items[i];
                if (item.callback)
                {
                    Logger::info("Menu item clicked: {}", item.label);
                    item.callback();
                }
                break;
            }
        }
    }
}

int32
Menu::getSelectedIndex() const
{
    return m_selectedIndex;
}

void
Menu::setSelectedIndex(int32 index)
{
    if (index >= 0 && index < static_cast<int32>(m_items.size()))
    {
        m_selectedIndex = index;
    }
}

void
Menu::updateScreenSize(int32 width, int32 height)
{
    m_screenWidth  = width;
    m_screenHeight = height;
    calculatePositions();
}

void
Menu::calculatePositions()
{
    m_itemPositions.clear();

    if (m_items.empty())
        return;

    // Calculate total height of menu
    float32 totalHeight = static_cast<float32>(m_items.size()) * m_itemSpacing;

    // Start Y position to center menu vertically
    float32 startY = (static_cast<float32>(m_screenHeight) - totalHeight) / 2.0f;

    Logger::info("HOW MUCH SPACE USED? H S: {}, H C: {}", m_screenHeight, startY);

    for (size_t i = 0; i < m_items.size(); ++i)
    {
        // Calculate approximate text width (rough estimate)
        float32 textWidth = static_cast<float32>(m_items[i].label.length()) * m_fontSize * 0.5f;

        // Center horizontally
        float32 x = (static_cast<float32>(m_screenWidth) - textWidth) / 2.0f;
        float32 y = startY - static_cast<float32>(i) * m_itemSpacing;

        m_itemPositions.push_back(glm::vec2(x, y));
    }
}

bool
Menu::isMouseOverItem(int32 index, float32 mouseX, float32 mouseY) const
{
    if (index < 0 || index >= static_cast<int32>(m_itemPositions.size()))
        return false;

    if (!m_textRenderer)
        return false;

    const auto& pos = m_itemPositions[static_cast<size_t>(index)];

    // Convert mouse Y to OpenGL coordinates (flip Y axis)
    float32 glMouseY = static_cast<float32>(m_screenHeight) - mouseY;

    // Calculate text bounds using the same centering logic as render
    String displayText = m_items[static_cast<size_t>(index)].label;
    if (static_cast<int32>(index) == m_selectedIndex && m_items[static_cast<size_t>(index)].enabled)
    {
        displayText = "> " + displayText + " <";
    }

    // Calculate actual text width using TextRenderer
    float32 displayWidth = m_textRenderer->getTextWidth(displayText, 1.0f);
    float32 renderX      = (static_cast<float32>(m_screenWidth) - displayWidth) / 2.0f;

    // Text height - the text extends upward from baseline (pos.y)
    float32 textTop    = pos.y + m_fontSize;
    float32 textBottom = pos.y - m_fontSize * 0.3f;  // Allow some space below baseline

    // Check if mouse is within text bounds
    bool withinX = mouseX >= renderX && mouseX <= renderX + displayWidth;
    bool withinY = glMouseY >= textBottom && glMouseY <= textTop;

    return withinX && withinY;
}

void
Menu::setTitle(const String& title)
{
    m_title = title;
}

void
Menu::setVersion(const String& version)
{
    m_version = version;
}

}  // namespace deadcode
