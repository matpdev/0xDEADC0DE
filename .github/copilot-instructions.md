# Copilot Instructions - 0xDEADC0DE

## Project Context
Text-based RPG with OpenGL rendering. C++20, CMake, vcpkg. Three-tier architecture: `deadcode_engine` (graphics/audio/input), `deadcode_game` (gameplay logic), `deadcode_rpg` (executable).

## Code Style

**Formatting (GNU)**
- Braces on new line, indent 4 spaces
- 100 char line limit
- Spaces around operators, after commas
- Use clang-format (config in `.clang-format`)

**Naming**
- Classes: `PascalCase` (Application, GameState)
- Functions/methods: `camelCase` (initialize, processInput)
- Variables: `camelCase` (deltaTime, isRunning)
- Members: `m_` prefix (m_window, m_config)
- Constants: `UPPER_SNAKE_CASE` or `k` prefix (MAX_PLAYERS, kDefaultFPS)
- Files: Match class name (Application.hpp/cpp)

**Headers**
```cpp
#pragma once
namespace deadcode { class X; }
```

**Includes Order**
1. Own header
2. Project headers (`deadcode/...`)
3. Third-party (`<spdlog/...>`, `<GLFW/...>`)
4. Standard library (`<string>`, `<vector>`)

## Architecture Rules

**SOLID Principles**
- Single responsibility per class
- Depend on abstractions, not concrete types
- Interfaces for major systems (Window, Renderer, AudioManager)

**Patterns**
- Singleton: Application only
- Pimpl: For complex classes hiding implementation
- Manager classes: Coordinate subsystem resources
- Static loggers: `Logger::info()`, `Logger::error()`

**Dependencies**
- Engine: OpenGL, GLFW, GLEW, GLM, FreeType, spdlog, nlohmann_json, OpenAL, libvorbis
- Game layer: Depends on engine, no external deps
- Use `#include "deadcode/core/Types.hpp"` for common types

## File Organization

**Directories**
- `include/deadcode/`: Public headers (subsystem folders: core, graphics, input, audio, ui, game)
- `src/`: Implementation matching include structure
- `assets/`: config, shaders, fonts, sounds, music
- `tests/`: Unit tests matching src structure

**Header Structure**
```cpp
#pragma once
// Brief description
// Includes
namespace deadcode {
class ClassName {
public:
    // Public interface
private:
    // Members with m_ prefix
};
} // namespace deadcode
```

## Coding Conventions

**Modern C++20**
- Use `auto` when type is obvious
- Prefer `std::unique_ptr`/`shared_ptr` over raw pointers
- Use `constexpr` for compile-time constants
- Range-based for loops when possible
- Use `std::string_view` for read-only strings

**Error Handling**
- Exceptions for errors (std::runtime_error, std::invalid_argument)
- Log errors: `Logger::error("Message: {}", detail);`
- Return bool for success/fail in init methods

**Resource Management**
- RAII: Constructors acquire, destructors release
- ResourceManager for shared assets (textures, sounds)
- Smart pointers for ownership

**Logging**
- Use `Logger::trace/debug/info/warn/error/critical()`
- Format strings: `Logger::info("Player {} at ({}, {})", name, x, y);`
- Log initialization/shutdown of major systems

## Game-Specific

**Systems**
- Window: GLFW wrapper, event handling
- Renderer: OpenGL state, shader management, text rendering
- Input: Keyboard/mouse via GLFW, command parsing for text
- Audio: OpenAL for 3D positional audio, music streaming
- UI: Console-based with OpenGL text rendering
- Game: Entity-component, turn-based combat, quest system

**Entities**
- Base Entity class with virtual interface
- Character → Player/NPC/Enemy inheritance
- Components: Position, stats, inventory, equipment

**Configuration**
- JSON files in `assets/config/`
- Load via Config class: `Config::get<int>("game.targetFPS")`
- Dot notation for nested values

**Assets**
- Shaders: GLSL 330 core, in `assets/shaders/`
- Load via ResourceManager: `ResourceManager::loadShader("text")`

## Build System

**CMake Targets**
- `deadcode_engine`: Static library, all engine code
- `deadcode_game`: Static library, game logic, depends on engine
- `deadcode_rpg`: Executable, depends on both libraries

**Adding Files**
1. Create header in `include/deadcode/<subsystem>/`
2. Create source in `src/<subsystem>/`
3. Add to CMakeLists.txt in appropriate target
4. Include guard: `#pragma once`

**Scripts**
- `./scripts/build.sh [debug|release]`: Build project
- `./scripts/run.sh`: Run game
- `./scripts/format.sh`: Format all code
- `./scripts/lint.sh`: Run clang-tidy

## Documentation

**Comments**
- Doxygen for public API
- Brief description above classes/functions
- Explain *why*, not *what* (code shows what)

**Example**
```cpp
/**
 * @brief Manages the main application lifecycle
 * Singleton pattern ensures single instance coordination
 */
class Application {
public:
    /// Initialize application with command-line args
    bool initialize(int argc, char** argv);
```

## Common Patterns

**Initialization Pattern**
```cpp
bool ClassName::initialize() {
    Logger::info("Initializing ClassName...");
    // Setup logic
    if (failed) {
        Logger::error("ClassName init failed: {}", reason);
        return false;
    }
    return true;
}
```

**Update Pattern**
```cpp
void ClassName::update(float deltaTime) {
    // Use deltaTime for frame-independent movement
}
```

**Resource Cleanup**
```cpp
ClassName::~ClassName() {
    shutdown();
}

void ClassName::shutdown() {
    if (!m_initialized) return;
    // Cleanup in reverse order of initialization
    m_initialized = false;
}
```

## Prohibited

- No `using namespace std;`
- No raw `new`/`delete` (use smart pointers)
- No `#define` for constants (use `constexpr`)
- No C-style casts (use `static_cast`, `dynamic_cast`)
- No magic numbers (use named constants)
- No platform-specific code without `#ifdef` guards

## Quick Reference

- Namespace: `deadcode`
- Logger: `Logger::info("message")`
- Config: `Config::get<T>("path.to.key")`
- Types: Use `deadcode::i32`, `deadcode::f32` from Types.hpp
- Error handling: Exceptions + logging
- Code format: Run `./scripts/format.sh` before commit
