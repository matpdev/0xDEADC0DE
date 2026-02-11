/**
 * @file GameLoop.cpp
 * @brief Implementation of the Game Loop class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-09
 */
#include "deadcode/game/GameLoop.hpp"

#include "deadcode/core/Logger.hpp"
#include "deadcode/input/TextInput.hpp"

#include <glm/fwd.hpp>

#include <format>

#include <raylib.h>

namespace deadcode
{
GameLoop::GameLoop()
{
    Logger::info("Starting Game Loop...");
};

GameLoop::~GameLoop() {

};

bool
GameLoop::initialize(int32 screenWidth, int32 screenHeight)
{
    m_screenWidth  = screenWidth;
    m_screenHeight = screenHeight;

    m_textInput = new TextInput(m_screenWidth / 2, 50);

    return true;
}

void
GameLoop::run()
{
}

void
GameLoop::render(TextRenderer* textRenderer)
{
    textRenderer->renderText(std::format("HP"), 20, 20, 0.5F, glm::vec3(1.0F, 1.0F, 1.0F));
    textRenderer->renderText(std::format("ST"),
                             m_screenWidth - (textRenderer->getTextWidth("ST", 0.5F)) - 20, 20,
                             0.5F, glm::vec3(1.0F, 1.0F, 1.0F));

    m_textInput->render(80, m_screenHeight - 100);

    // textRenderer->renderText("Hello World",
    //                          (m_screenWidth / 2.0F) -
    //                              (textRenderer->getTextWidth("Hello World", 0.5F) / 2.0F),
    //                          m_screenHeight / 2, 0.5F, glm::vec3(1.0F, 1.0F, 1.0F));
}

void
GameLoop::update(float deltaTime)
{
}

void
GameLoop::onWindowResize(int32 screenWidth, int32 screenHeight)
{
}

void
GameLoop::shutdown()
{
}

void
GameLoop::handleInput(int key, int action)
{
}

}  // namespace deadcode
