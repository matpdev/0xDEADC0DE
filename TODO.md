# 0xDEADC0DE Text-Based RPG - Project Setup TODO

## Project Overview
Creating a Text-based RPG with OpenGL animations, controlled entirely via text commands.
- **Build System**: CMake with vcpkg
- **Architecture**: SOLID principles, GNU coding standards
- **Language**: C++20
- **Graphics**: OpenGL + GLFW + GLEW
- **Audio**: OpenAL

## Current Status Analysis
✅ **Completed:**
- CMakeLists.txt configuration (comprehensive build system)
- vcpkg.json manifest (all dependencies defined)
- .clang-format (GNU-based style)
- .clang-tidy (code quality rules)
- .editorconfig (editor consistency)
- .gitignore (comprehensive exclusions)
- CMake package configuration template

❌ **Missing:**
- All source code files (headers + implementation)
- Directory structure (src/, include/, assets/, etc.)
- Resource files (shaders, fonts, configs)
- Documentation files
- Build scripts
- Example game content

---

## Phase 1: Directory Structure Setup
Following GNU Standard Directory Layout

### 1.1 Core Directories
- [ ] `src/` - Source implementation files (.cpp)
  - [ ] `src/core/` - Core engine systems
  - [ ] `src/graphics/` - Graphics and rendering
  - [ ] `src/input/` - Input handling
  - [ ] `src/audio/` - Audio management
  - [ ] `src/ui/` - User interface
  - [ ] `src/game/` - Game logic
    - [ ] `src/game/entities/` - Entity system
    - [ ] `src/game/world/` - World and locations
    - [ ] `src/game/combat/` - Combat mechanics
    - [ ] `src/game/items/` - Items and inventory
    - [ ] `src/game/quest/` - Quest system
    - [ ] `src/game/dialogue/` - Dialogue system

### 1.2 Include Directory (Public Headers)
- [ ] `include/` - Public header files (.hpp)
  - [ ] `include/deadcode/` - Namespace root
    - [ ] `include/deadcode/core/` - Core headers
    - [ ] `include/deadcode/graphics/` - Graphics headers
    - [ ] `include/deadcode/input/` - Input headers
    - [ ] `include/deadcode/audio/` - Audio headers
    - [ ] `include/deadcode/ui/` - UI headers
    - [ ] `include/deadcode/game/` - Game headers
      - [ ] `include/deadcode/game/entities/`
      - [ ] `include/deadcode/game/world/`
      - [ ] `include/deadcode/game/combat/`
      - [ ] `include/deadcode/game/items/`
      - [ ] `include/deadcode/game/quest/`
      - [ ] `include/deadcode/game/dialogue/`

### 1.3 Assets Directory
- [ ] `assets/` - Game resources
  - [ ] `assets/shaders/` - GLSL shader files
  - [ ] `assets/fonts/` - TTF/OTF font files
  - [ ] `assets/sounds/` - Sound effects (.ogg)
  - [ ] `assets/music/` - Background music (.ogg)
  - [ ] `assets/data/` - Game data files (.json)
    - [ ] `assets/data/items/` - Item definitions
    - [ ] `assets/data/characters/` - Character data
    - [ ] `assets/data/quests/` - Quest definitions
    - [ ] `assets/data/world/` - World/location data
    - [ ] `assets/data/dialogue/` - Dialogue trees
  - [ ] `assets/config/` - Configuration files
  - [ ] `assets/textures/` - ASCII art and effects (optional)

### 1.4 Testing Directory
- [ ] `tests/` - Unit tests
  - [ ] `tests/core/` - Core system tests
  - [ ] `tests/game/` - Game logic tests
  - [ ] `tests/integration/` - Integration tests

### 1.5 Documentation Directory
- [ ] `docs/` - Project documentation
  - [ ] `docs/api/` - API documentation (Doxygen)
  - [ ] `docs/design/` - Design documents
  - [ ] `docs/guides/` - User/developer guides

### 1.6 Scripts Directory
- [ ] `scripts/` - Utility scripts
  - [ ] `scripts/build.sh` - Build helper
  - [ ] `scripts/run.sh` - Run helper
  - [ ] `scripts/format.sh` - Code formatting
  - [ ] `scripts/lint.sh` - Linting
  - [ ] `scripts/install-deps.sh` - Dependency installer

### 1.7 External Dependencies (if needed)
- [ ] `external/` - Third-party code (if not via vcpkg)
  - [ ] `external/stb/` - stb_image, stb_truetype headers

---

## Phase 2: Core Engine Implementation

### 2.1 Core System Headers & Sources
- [ ] `include/deadcode/core/Application.hpp` + `src/core/Application.cpp`
  - Main application class, game loop controller
- [ ] `include/deadcode/core/Logger.hpp` + `src/core/Logger.cpp`
  - Wrapper around spdlog, custom formatting
- [ ] `include/deadcode/core/Config.hpp` + `src/core/Config.cpp`
  - Configuration management (JSON-based)
- [ ] `include/deadcode/core/Timer.hpp` + `src/core/Timer.cpp`
  - High-precision timing, delta time calculation
- [ ] `include/deadcode/core/ResourceManager.hpp` + `src/core/ResourceManager.cpp`
  - Resource loading, caching, lifecycle management
- [ ] `include/deadcode/core/Types.hpp`
  - Common type definitions, aliases
- [ ] `include/deadcode/core/Version.hpp`
  - Version information (generated from CMake)

### 2.2 Graphics System Headers & Sources
- [ ] `include/deadcode/graphics/Window.hpp` + `src/graphics/Window.cpp`
  - GLFW window wrapper, event handling
- [ ] `include/deadcode/graphics/Renderer.hpp` + `src/graphics/Renderer.cpp`
  - Main rendering system, draw calls orchestration
- [ ] `include/deadcode/graphics/Shader.hpp` + `src/graphics/Shader.cpp`
  - Shader loading, compilation, uniform management
- [ ] `include/deadcode/graphics/TextRenderer.hpp` + `src/graphics/TextRenderer.cpp`
  - FreeType text rendering, glyph caching
- [ ] `include/deadcode/graphics/AnimationSystem.hpp` + `src/graphics/AnimationSystem.cpp`
  - Text animations, effects, transitions
- [ ] `include/deadcode/graphics/EffectManager.hpp` + `src/graphics/EffectManager.cpp`
  - Visual effects for combat, dialogue, etc.
- [ ] `include/deadcode/graphics/Color.hpp`
  - Color utilities, palette definitions
- [ ] `include/deadcode/graphics/Buffer.hpp` + `src/graphics/Buffer.cpp`
  - OpenGL buffer wrappers (VBO, VAO, EBO)

### 2.3 Input System Headers & Sources
- [ ] `include/deadcode/input/InputManager.hpp` + `src/input/InputManager.cpp`
  - Keyboard input handling, event dispatching
- [ ] `include/deadcode/input/TextInput.hpp` + `src/input/TextInput.cpp`
  - Text input buffer, cursor management
- [ ] `include/deadcode/input/CommandParser.hpp` + `src/input/CommandParser.cpp`
  - Command parsing, tokenization, validation
- [ ] `include/deadcode/input/KeyBindings.hpp` + `src/input/KeyBindings.cpp`
  - Configurable key bindings

### 2.4 Audio System Headers & Sources
- [ ] `include/deadcode/audio/AudioManager.hpp` + `src/audio/AudioManager.cpp`
  - OpenAL context, listener management
- [ ] `include/deadcode/audio/SoundEffect.hpp` + `src/audio/SoundEffect.cpp`
  - Sound effect loading, playback
- [ ] `include/deadcode/audio/MusicPlayer.hpp` + `src/audio/MusicPlayer.cpp`
  - Background music streaming, volume control
- [ ] `include/deadcode/audio/AudioBuffer.hpp` + `src/audio/AudioBuffer.cpp`
  - Audio buffer management, Vorbis decoding

### 2.5 UI System Headers & Sources
- [ ] `include/deadcode/ui/UIManager.hpp` + `src/ui/UIManager.cpp`
  - UI state management, layout
- [ ] `include/deadcode/ui/Console.hpp` + `src/ui/Console.cpp`
  - Command console, history, scrolling
- [ ] `include/deadcode/ui/TextBox.hpp` + `src/ui/TextBox.cpp`
  - Text display widget, word wrapping
- [ ] `include/deadcode/ui/Menu.hpp` + `src/ui/Menu.cpp`
  - Menu system, navigation
- [ ] `include/deadcode/ui/Widget.hpp` + `src/ui/Widget.cpp`
  - Base widget class

---

## Phase 3: Game Logic Implementation

### 3.1 Game Core Headers & Sources
- [ ] `include/deadcode/game/GameState.hpp` + `src/game/GameState.cpp`
  - Game state machine, transitions
- [ ] `include/deadcode/game/GameLoop.hpp` + `src/game/GameLoop.cpp`
  - Game loop control, update/render separation
- [ ] `include/deadcode/game/SaveSystem.hpp` + `src/game/SaveSystem.cpp`
  - Save/load game state (JSON serialization)

### 3.2 Entity System Headers & Sources
- [ ] `include/deadcode/game/entities/Entity.hpp` + `src/game/entities/Entity.cpp`
  - Base entity class (ECS-lite)
- [ ] `include/deadcode/game/entities/Character.hpp` + `src/game/entities/Character.cpp`
  - Character base (stats, attributes)
- [ ] `include/deadcode/game/entities/Player.hpp` + `src/game/entities/Player.cpp`
  - Player character class
- [ ] `include/deadcode/game/entities/NPC.hpp` + `src/game/entities/NPC.cpp`
  - Non-player character class
- [ ] `include/deadcode/game/entities/Enemy.hpp` + `src/game/entities/Enemy.cpp`
  - Enemy character class
- [ ] `include/deadcode/game/entities/Stats.hpp`
  - Stats structure (HP, MP, STR, DEX, etc.)
- [ ] `include/deadcode/game/entities/Attributes.hpp`
  - Character attributes, modifiers

### 3.3 World System Headers & Sources
- [ ] `include/deadcode/game/world/World.hpp` + `src/game/world/World.cpp`
  - World container, location management
- [ ] `include/deadcode/game/world/Location.hpp` + `src/game/world/Location.cpp`
  - Location/area class
- [ ] `include/deadcode/game/world/Room.hpp` + `src/game/world/Room.cpp`
  - Individual room/scene class
- [ ] `include/deadcode/game/world/Connection.hpp`
  - Room connections, exits

### 3.4 Combat System Headers & Sources
- [ ] `include/deadcode/game/combat/CombatSystem.hpp` + `src/game/combat/CombatSystem.cpp`
  - Turn-based combat engine
- [ ] `include/deadcode/game/combat/Ability.hpp` + `src/game/combat/Ability.cpp`
  - Skills and abilities
- [ ] `include/deadcode/game/combat/StatusEffect.hpp` + `src/game/combat/StatusEffect.cpp`
  - Status effects (buffs/debuffs)
- [ ] `include/deadcode/game/combat/DamageCalculator.hpp` + `src/game/combat/DamageCalculator.cpp`
  - Damage calculation formulas

### 3.5 Item System Headers & Sources
- [ ] `include/deadcode/game/items/Item.hpp` + `src/game/items/Item.cpp`
  - Base item class
- [ ] `include/deadcode/game/items/Inventory.hpp` + `src/game/items/Inventory.cpp`
  - Inventory management
- [ ] `include/deadcode/game/items/Equipment.hpp` + `src/game/items/Equipment.cpp`
  - Equipment system, slots
- [ ] `include/deadcode/game/items/Consumable.hpp` + `src/game/items/Consumable.cpp`
  - Consumable items (potions, etc.)
- [ ] `include/deadcode/game/items/Weapon.hpp` + `src/game/items/Weapon.cpp`
  - Weapon items
- [ ] `include/deadcode/game/items/Armor.hpp` + `src/game/items/Armor.cpp`
  - Armor items

### 3.6 Quest System Headers & Sources
- [ ] `include/deadcode/game/quest/Quest.hpp` + `src/game/quest/Quest.cpp`
  - Quest definition
- [ ] `include/deadcode/game/quest/QuestManager.hpp` + `src/game/quest/QuestManager.cpp`
  - Quest tracking, completion
- [ ] `include/deadcode/game/quest/Objective.hpp` + `src/game/quest/Objective.cpp`
  - Quest objectives

### 3.7 Dialogue System Headers & Sources
- [ ] `include/deadcode/game/dialogue/DialogueSystem.hpp` + `src/game/dialogue/DialogueSystem.cpp`
  - Dialogue state machine
- [ ] `include/deadcode/game/dialogue/DialogueTree.hpp` + `src/game/dialogue/DialogueTree.cpp`
  - Dialogue tree structure
- [ ] `include/deadcode/game/dialogue/DialogueNode.hpp` + `src/game/dialogue/DialogueNode.cpp`
  - Individual dialogue node
- [ ] `include/deadcode/game/dialogue/DialogueChoice.hpp`
  - Player dialogue choices

---

## Phase 4: Shader and Assets

### 4.1 Shader Files
- [ ] `assets/shaders/text.vert` - Text vertex shader
- [ ] `assets/shaders/text.frag` - Text fragment shader
- [ ] `assets/shaders/effect.vert` - Effect vertex shader
- [ ] `assets/shaders/effect.frag` - Effect fragment shader (particle-like)
- [ ] `assets/shaders/color.vert` - Simple color shader
- [ ] `assets/shaders/color.frag` - Color fragment shader

### 4.2 Font Files
- [ ] `assets/fonts/monospace.ttf` - Main monospace font
- [ ] `assets/fonts/title.ttf` - Title/header font
- [ ] `assets/fonts/README.md` - Font license information

### 4.3 Configuration Files
- [ ] `assets/config/game.json` - Game settings
- [ ] `assets/config/graphics.json` - Graphics settings
- [ ] `assets/config/audio.json` - Audio settings
- [ ] `assets/config/keybindings.json` - Default key bindings
- [ ] `assets/config/colors.json` - Color palette definitions

### 4.4 Game Data Files
- [ ] `assets/data/items/weapons.json` - Weapon definitions
- [ ] `assets/data/items/armor.json` - Armor definitions
- [ ] `assets/data/items/consumables.json` - Consumable items
- [ ] `assets/data/characters/player_classes.json` - Player classes
- [ ] `assets/data/characters/enemies.json` - Enemy definitions
- [ ] `assets/data/characters/npcs.json` - NPC definitions
- [ ] `assets/data/quests/main_quest.json` - Main quest line
- [ ] `assets/data/quests/side_quests.json` - Side quests
- [ ] `assets/data/world/locations.json` - World locations
- [ ] `assets/data/world/rooms.json` - Room definitions
- [ ] `assets/data/dialogue/npc_dialogue.json` - NPC dialogues
- [ ] `assets/data/abilities.json` - Abilities and skills
- [ ] `assets/data/status_effects.json` - Status effect definitions

### 4.5 Audio Assets (Placeholder or Open Source)
- [ ] `assets/sounds/menu_select.ogg` - Menu selection sound
- [ ] `assets/sounds/menu_confirm.ogg` - Confirmation sound
- [ ] `assets/sounds/attack.ogg` - Attack sound
- [ ] `assets/sounds/hit.ogg` - Hit sound
- [ ] `assets/sounds/heal.ogg` - Healing sound
- [ ] `assets/music/menu_theme.ogg` - Menu music
- [ ] `assets/music/exploration_theme.ogg` - Exploration music
- [ ] `assets/music/combat_theme.ogg` - Combat music
- [ ] `assets/sounds/README.md` - Audio license information

---

## Phase 5: Main Application

### 5.1 Entry Point
- [ ] `src/main.cpp` - Application entry point
  - Initialize engine
  - Create application instance
  - Run main game loop
  - Cleanup

---

## Phase 6: Testing

### 6.1 Core Tests
- [ ] `tests/main.cpp` - Test runner main
- [ ] `tests/core/test_logger.cpp` - Logger tests
- [ ] `tests/core/test_config.cpp` - Config tests
- [ ] `tests/core/test_timer.cpp` - Timer tests
- [ ] `tests/core/test_resource_manager.cpp` - Resource manager tests

### 6.2 Game Logic Tests
- [ ] `tests/game/test_entity.cpp` - Entity system tests
- [ ] `tests/game/test_combat.cpp` - Combat system tests
- [ ] `tests/game/test_inventory.cpp` - Inventory tests
- [ ] `tests/game/test_quest.cpp` - Quest system tests
- [ ] `tests/game/test_dialogue.cpp` - Dialogue system tests
- [ ] `tests/game/test_save_system.cpp` - Save/load tests

### 6.3 Integration Tests
- [ ] `tests/integration/test_game_flow.cpp` - Full game flow test
- [ ] `tests/integration/test_combat_flow.cpp` - Combat scenario tests

---

## Phase 7: Documentation

### 7.1 User Documentation
- [ ] `README.md` - Project overview, build instructions
- [ ] `docs/guides/getting_started.md` - Getting started guide
- [ ] `docs/guides/gameplay.md` - Gameplay guide
- [ ] `docs/guides/commands.md` - Command reference

### 7.2 Developer Documentation
- [ ] `docs/CONTRIBUTING.md` - Contribution guidelines
- [ ] `docs/ARCHITECTURE.md` - Architecture overview
- [ ] `docs/CODING_STYLE.md` - Coding style guide
- [ ] `docs/design/entity_system.md` - Entity system design
- [ ] `docs/design/combat_system.md` - Combat system design
- [ ] `docs/design/animation_system.md` - Animation system design
- [ ] `docs/design/data_format.md` - Data file format specifications

### 7.3 Doxygen Configuration
- [ ] `docs/Doxyfile.in` - Doxygen configuration template

### 7.4 License and Legal
- [ ] `LICENSE` - Project license (GPL-3.0 or chosen license)
- [ ] `NOTICE` - Third-party attributions

---

## Phase 8: Build and Utility Scripts

### 8.1 Build Scripts
- [ ] `scripts/build.sh` - Unix build script
  ```bash
  # Configure and build with vcpkg
  # Support different build types
  ```
- [ ] `scripts/build.bat` - Windows build script (optional)
- [ ] `scripts/clean.sh` - Clean build artifacts

### 8.2 Development Scripts
- [ ] `scripts/run.sh` - Run the game
- [ ] `scripts/debug.sh` - Run with debugger
- [ ] `scripts/format.sh` - Format code with clang-format
- [ ] `scripts/lint.sh` - Lint code with clang-tidy
- [ ] `scripts/test.sh` - Run tests
- [ ] `scripts/install-deps.sh` - Install system dependencies

### 8.3 CI/CD Configuration (Optional)
- [ ] `.github/workflows/build.yml` - GitHub Actions build workflow
- [ ] `.github/workflows/test.yml` - GitHub Actions test workflow

---

## Phase 9: External Dependencies Setup

### 9.1 STB Headers (if not using vcpkg version)
- [ ] `external/stb/stb_image.h` - Image loading
- [ ] `external/stb/stb_truetype.h` - TrueType font loading
- [ ] `external/stb/LICENSE` - STB license

### 9.2 vcpkg Integration
- [ ] Verify vcpkg installation
- [ ] Test dependency resolution
- [ ] Document vcpkg usage

---

## Phase 10: Initial Implementation & Testing

### 10.1 Minimal Viable Product
- [ ] Implement basic application skeleton
- [ ] Create simple window with text rendering
- [ ] Implement basic command input
- [ ] Create simple "hello world" scene
- [ ] Test build on target platforms

### 10.2 Core Systems Integration
- [ ] Integrate logger with all systems
- [ ] Setup resource loading pipeline
- [ ] Test shader compilation
- [ ] Test font rendering
- [ ] Verify audio initialization

### 10.3 Basic Game Loop
- [ ] Implement simple room navigation
- [ ] Create basic inventory system
- [ ] Implement simple combat encounter
- [ ] Test save/load functionality

---

## Phase 11: Polish and Configuration

### 11.1 Configuration Validation
- [ ] Validate all JSON schemas
- [ ] Test configuration loading
- [ ] Document all configuration options

### 11.2 Error Handling
- [ ] Implement comprehensive error handling
- [ ] Add error recovery mechanisms
- [ ] Create user-friendly error messages

### 11.3 Performance Optimization
- [ ] Profile rendering performance
- [ ] Optimize text rendering
- [ ] Optimize resource loading
- [ ] Add resource caching

---

## Priority Order Recommendation

1. **CRITICAL (Do First)**:
   - Directory structure (Phase 1)
   - Core engine headers (Phase 2.1, 2.2)
   - Main application entry point (Phase 5)
   - Basic shaders and config (Phase 4.1, 4.3)

2. **HIGH PRIORITY (Core Functionality)**:
   - Graphics system implementation (Phase 2.2)
   - Input system (Phase 2.3)
   - Game state management (Phase 3.1)
   - Entity system basics (Phase 3.2)
   - Basic world system (Phase 3.3)

3. **MEDIUM PRIORITY (Game Features)**:
   - Combat system (Phase 3.4)
   - Item/Inventory system (Phase 3.5)
   - Dialogue system (Phase 3.7)
   - Audio system (Phase 2.4)
   - Game data files (Phase 4.4)

4. **LOWER PRIORITY (Enhancement)**:
   - Quest system (Phase 3.6)
   - Testing (Phase 6)
   - Documentation (Phase 7)
   - Build scripts (Phase 8)
   - Audio assets (Phase 4.5)

5. **FINAL (Polish)**:
   - CI/CD setup (Phase 8.3)
   - Full documentation (Phase 7)
   - Performance optimization (Phase 11.3)

---

## Estimated File Count
- **Headers**: ~60 files
- **Source files**: ~60 files
- **Shaders**: ~6 files
- **Config files**: ~5 files
- **Game data files**: ~15 files
- **Tests**: ~15 files
- **Documentation**: ~10 files
- **Scripts**: ~10 files
- **Total**: ~180+ files

---

## Notes on SOLID Principles Implementation

- **Single Responsibility**: Each class handles one aspect (e.g., TextRenderer only renders text)
- **Open/Closed**: Use inheritance for entities, items (base classes extensible)
- **Liskov Substitution**: All entities derive from Entity base class
- **Interface Segregation**: Separate interfaces for different behaviors
- **Dependency Inversion**: Use abstractions (interfaces) not concrete implementations

## Notes on GNU Standards

- **Directory Layout**: Follow GNU standard (bin/, lib/, include/, share/)
- **Coding Style**: GNU brace style (configured in .clang-format)
- **Naming**: Use camelCase with m_ prefix for members (configured in .clang-tidy)
- **Documentation**: Comprehensive inline comments, Doxygen documentation

---

**Last Updated**: January 21, 2026
**Status**: Planning Phase - Ready to Begin Implementation
