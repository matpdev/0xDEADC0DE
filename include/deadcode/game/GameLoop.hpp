/**
 * @file GameLoop.hpp
 * @brief Manage Game Loop Environment
 *
 * This manage all Game Loop environment
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-09
 */

#include "deadcode/core/Types.hpp"
#include "deadcode/graphics/TextRenderer.hpp"
#include "deadcode/input/InputManager.hpp"

#include <functional>
#include <memory>

#include <raylib.h>

namespace deadcode
{
class GameLoop
{
public:
    GameLoop();

    ~GameLoop();

    bool initialize(int32 screenWidth, int32 screenHeight);

    void run();

    void render(TextRenderer* textRenderer);
    void update(float deltatime);

    void onWindowResize(int32 screenWidth, int32 screenHeight);

    void shutdown();

    void handleInput(int key, int action);

private:
    bool m_started{false};
    bool m_paused{false};

    int32 m_screenWidth{1920};
    int32 m_screenHeight{1080};
};
}  // namespace deadcode
