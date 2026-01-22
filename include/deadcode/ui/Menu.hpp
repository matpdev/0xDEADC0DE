/**
 * @file Menu.hpp
 * @brief Interactive menu system with keyboard and mouse support
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-22
 */

#pragma once

#include "deadcode/core/Types.hpp"

#include <glm/glm.hpp>

#include <functional>
#include <string>
#include <vector>

namespace deadcode
{

class TextRenderer;

/**
 * @brief Menu item with label and callback
 */
struct MenuItem
{
    String label;
    std::function<void()> callback;
    bool enabled{true};
};

/**
 * @brief Interactive menu system
 *
 * Provides a centered menu with keyboard and mouse navigation.
 * Supports arrow key selection and mouse hover/click.
 */
class Menu
{
public:
    /**
     * @brief Constructor
     */
    Menu();

    /**
     * @brief Destructor
     */
    ~Menu();

    /**
     * @brief Initialize menu system
     *
     * @param screenWidth Screen width in pixels
     * @param screenHeight Screen height in pixels
     * @return true if successful
     */
    bool initialize(int32 screenWidth, int32 screenHeight);

    /**
     * @brief Add menu item
     *
     * @param label Menu item text
     * @param callback Function to call when item is selected
     * @param enabled Whether item is initially enabled
     */
    void addItem(const String& label, std::function<void()> callback, bool enabled = true);

    /**
     * @brief Clear all menu items
     */
    void clear();

    /**
     * @brief Update menu state
     *
     * @param deltaTime Time since last frame
     */
    void update(float deltaTime);

    /**
     * @brief Render menu to screen
     *
     * @param textRenderer Text rendering system
     */
    void render(TextRenderer* textRenderer);

    /**
     * @brief Handle keyboard input
     *
     * @param key GLFW key code
     * @param action GLFW action (press, release, repeat)
     */
    void handleKeyboard(int key, int action);

    /**
     * @brief Handle mouse movement
     *
     * @param x Mouse X position in screen coordinates
     * @param y Mouse Y position in screen coordinates
     */
    void handleMouseMove(float32 x, float32 y);

    /**
     * @brief Handle mouse click
     *
     * @param button Mouse button
     * @param action GLFW action (press, release)
     * @param x Mouse X position
     * @param y Mouse Y position
     */
    void handleMouseClick(int button, int action, float32 x, float32 y);

    /**
     * @brief Get current selected index
     * @return Index of selected item
     */
    [[nodiscard]] int32 getSelectedIndex() const;

    /**
     * @brief Set selected index
     * @param index New selected index
     */
    void setSelectedIndex(int32 index);

    /**
     * @brief Update screen dimensions
     *
     * @param width New screen width
     * @param height New screen height
     */
    void updateScreenSize(int32 width, int32 height);

    /**
     * @brief Set menu title
     *
     * @param title Title text to display above menu items
     */
    void setTitle(const String& title);

    /**
     * @brief Set version text
     *
     * @param version Version text to display
     */
    void setVersion(const String& version);

    // Delete copy and move
    Menu(const Menu&)            = delete;
    Menu& operator=(const Menu&) = delete;

private:
    /**
     * @brief Calculate menu item positions
     */
    void calculatePositions();

    /**
     * @brief Check if mouse is over an item
     *
     * @param index Item index
     * @param mouseX Mouse X position
     * @param mouseY Mouse Y position
     * @return true if mouse is over the item
     */
    bool isMouseOverItem(int32 index, float32 mouseX, float32 mouseY) const;

    std::vector<MenuItem> m_items;
    int32 m_selectedIndex{0};

    int32 m_screenWidth{800};
    int32 m_screenHeight{600};

    float32 m_fontSize{48.0f};
    float32 m_itemSpacing{60.0f};

    // Item positions (calculated based on screen size)
    std::vector<glm::vec2> m_itemPositions;
    float32 m_itemHeight{50.0f};

    // Title and version
    String m_title;
    String m_version;
    float32 m_titleFontSize{64.0f};
    float32 m_versionFontSize{32.0f};
};

}  // namespace deadcode
