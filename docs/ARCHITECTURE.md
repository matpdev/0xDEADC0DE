# 0xDEADC0DE Architecture

## Overview

0xDEADC0DE is a text-based RPG engine built with modern C++20, following SOLID principles and GNU coding standards. The architecture is modular, separating engine concerns from game logic.

## System Architecture

### High-Level Architecture

```
┌─────────────────────────────────────────────────────────┐
│                   deadcode_rpg (Executable)             │
│                     - Application Entry                 │
│                     - Main Game Loop                    │
└────────────────┬────────────────────────────────────────┘
                 │
    ┌────────────┴────────────┐
    │                         │
┌───▼──────────────────┐ ┌───▼──────────────────┐
│  deadcode_game       │ │  deadcode_engine     │
│  (Game Library)      │ │  (Engine Library)    │
│                      │ │                      │
│  - Entities          │ │  - Core Systems      │
│  - World/Locations   │ │  - Graphics          │
│  - Combat            │ │  - Input             │
│  - Items/Inventory   │ │  - Audio             │
│  - Quests            │ │  - UI Framework      │
│  - Dialogue          │ │  - Resource Mgmt     │
└──────────────────────┘ └──────────────────────┘
```

### Design Principles

#### SOLID Principles Implementation

1. **Single Responsibility Principle (SRP)**
   - Each class has one well-defined responsibility
   - `TextRenderer` only handles text rendering
   - `AudioManager` only manages audio resources
   - `InputManager` only processes input events

2. **Open/Closed Principle (OCP)**
   - Systems are open for extension, closed for modification
   - Entity system uses inheritance: `Entity` → `Character` → `Player`/`NPC`/`Enemy`
   - Item system: `Item` → `Weapon`/`Armor`/`Consumable`
   - Effects and abilities use polymorphism

3. **Liskov Substitution Principle (LSP)**
   - Derived classes can substitute base classes
   - Any `Character*` can be used where `Entity*` is expected
   - All `Item*` types behave consistently

4. **Interface Segregation Principle (ISP)**
   - Interfaces are focused and minimal
   - `Renderable` interface only has `render()`
   - `Updateable` interface only has `update()`
   - `Serializable` interface only has `serialize()`/`deserialize()`

5. **Dependency Inversion Principle (DIP)**
   - Depend on abstractions, not concretions
   - `GameState` depends on `IEntityManager` interface, not concrete implementation
   - `Renderer` uses abstract `IShaderProgram` interface
   - Resource loading through `IResourceLoader` abstraction

## Engine Layer

### Core Systems

#### Application (`src/core/Application.cpp`)
- **Purpose**: Main application lifecycle management
- **Responsibilities**:
  - Initialize all subsystems
  - Manage main game loop
  - Handle high-level state transitions
  - Coordinate shutdown
- **Dependencies**: Window, Renderer, InputManager, AudioManager

#### Logger (`src/core/Logger.cpp`)
- **Purpose**: Centralized logging system
- **Implementation**: Wrapper around spdlog
- **Features**:
  - Multiple log levels (trace, debug, info, warn, error, critical)
  - Console and file output
  - Colored output for terminals
  - Thread-safe logging

#### Configuration (`src/core/Config.cpp`)
- **Purpose**: Configuration management
- **Format**: JSON-based configuration files
- **Features**:
  - Hot-reload configuration
  - Type-safe value access
  - Default value handling
  - Validation

#### Timer (`src/core/Timer.cpp`)
- **Purpose**: High-precision timing
- **Features**:
  - Delta time calculation
  - FPS tracking
  - Frame time smoothing
  - Pause/resume support

#### ResourceManager (`src/core/ResourceManager.cpp`)
- **Purpose**: Resource lifecycle management
- **Pattern**: Resource pool + caching
- **Managed Resources**:
  - Textures
  - Fonts
  - Shaders
  - Audio files
  - JSON data files

### Graphics Systems

#### Window (`src/graphics/Window.cpp`)
- **Purpose**: Window and context management
- **Backend**: GLFW
- **Features**:
  - Window creation/destruction
  - Event polling
  - Context management
  - Fullscreen toggle
  - Resolution changes

#### Renderer (`src/graphics/Renderer.cpp`)
- **Purpose**: Central rendering coordinator
- **Architecture**: Command pattern for render operations
- **Pipeline**:
  1. Clear buffers
  2. Update camera/viewport
  3. Submit render commands
  4. Execute commands in batches
  5. Swap buffers

#### Shader (`src/graphics/Shader.cpp`)
- **Purpose**: Shader program management
- **Features**:
  - Compilation and linking
  - Uniform management
  - Automatic uniform location caching
  - Error reporting

#### TextRenderer (`src/graphics/TextRenderer.cpp`)
- **Purpose**: Text rendering with FreeType
- **Implementation**:
  - Glyph atlas generation
  - SDF (Signed Distance Field) rendering
  - Kerning and line height
  - Rich text formatting (color, styles)
- **Optimizations**:
  - Glyph caching
  - Batch rendering
  - Instanced drawing

#### AnimationSystem (`src/graphics/AnimationSystem.cpp`)
- **Purpose**: Text animation effects
- **Effects**:
  - Type-writer effect
  - Fade in/out
  - Wave/shake effects
  - Color transitions
  - Particle-like effects
- **Implementation**: Keyframe-based animation

### Input Systems

#### InputManager (`src/input/InputManager.cpp`)
- **Purpose**: Keyboard and mouse input handling
- **Pattern**: Observer pattern for input events
- **Features**:
  - Key state tracking
  - Input buffering
  - Key repeat handling
  - Custom key bindings

#### TextInput (`src/input/TextInput.cpp`)
- **Purpose**: Text input buffer management
- **Features**:
  - Cursor movement
  - Text selection
  - Copy/paste
  - Input history
  - Auto-completion hints

#### CommandParser (`src/input/CommandParser.cpp`)
- **Purpose**: Parse and validate text commands
- **Implementation**:
  - Tokenization
  - Command lookup
  - Argument validation
  - Fuzzy matching for typos
- **Example**: `"go north"` → `{command: "go", args: ["north"]}`

### Audio Systems

#### AudioManager (`src/audio/AudioManager.cpp`)
- **Purpose**: Audio engine management
- **Backend**: OpenAL Soft
- **Features**:
  - 3D positional audio
  - Volume control
  - Audio mixing
  - Resource management

#### SoundEffect (`src/audio/SoundEffect.cpp`)
- **Purpose**: Short sound effects
- **Implementation**: Preloaded buffers
- **Usage**: UI sounds, combat sounds, ambient effects

#### MusicPlayer (`src/audio/MusicPlayer.cpp`)
- **Purpose**: Background music streaming
- **Implementation**: Buffer streaming for large files
- **Features**:
  - Crossfade transitions
  - Loop support
  - Volume fade in/out

### UI Systems

#### UIManager (`src/ui/UIManager.cpp`)
- **Purpose**: UI layout and state management
- **Architecture**: Widget tree
- **Responsibilities**:
  - Widget lifecycle
  - Layout calculation
  - Focus management
  - Event dispatching

#### Console (`src/ui/Console.cpp`)
- **Purpose**: Command console implementation
- **Features**:
  - Command history
  - Scrollback buffer
  - Auto-completion
  - Syntax highlighting

#### TextBox (`src/ui/TextBox.cpp`)
- **Purpose**: Multi-line text display widget
- **Features**:
  - Word wrapping
  - Scrolling
  - Text formatting
  - Selectable text

#### Menu (`src/ui/Menu.cpp`)
- **Purpose**: Menu system
- **Types**:
  - Main menu
  - Pause menu
  - Inventory menu
  - Dialogue choices
- **Features**:
  - Keyboard/mouse navigation
  - Animations
  - Custom layouts

## Game Layer

### Game Core

#### GameState (`src/game/GameState.cpp`)
- **Purpose**: Game state machine
- **States**:
  - MainMenu
  - Playing
  - Combat
  - Dialogue
  - Inventory
  - Paused
  - GameOver
- **Pattern**: State pattern

#### GameLoop (`src/game/GameLoop.cpp`)
- **Purpose**: Game-specific loop logic
- **Phases**:
  1. Process input
  2. Update game state
  3. Update entities
  4. Update animations
  5. Render frame
  6. Audio update

#### SaveSystem (`src/game/SaveSystem.cpp`)
- **Purpose**: Save/load game state
- **Format**: JSON
- **Saved Data**:
  - Player state
  - Inventory
  - Quest progress
  - World state
  - Visited locations

### Entity System

```
                    Entity (Base)
                       │
           ┌───────────┴───────────┐
           │                       │
      Character                 Item
           │                       │
    ┌──────┴──────┐       ┌───────┴────────┐
    │      │      │       │       │        │
 Player  NPC   Enemy   Weapon  Armor  Consumable
```

#### Entity (`src/game/entities/Entity.cpp`)
- **Purpose**: Base entity class
- **Common Properties**:
  - ID
  - Name
  - Description
  - Position
  - State flags

#### Character (`src/game/entities/Character.cpp`)
- **Purpose**: Base character class
- **Properties**:
  - Stats (HP, MP, STR, DEX, INT, etc.)
  - Level and experience
  - Equipment slots
  - Status effects
  - Inventory

#### Player (`src/game/entities/Player.cpp`)
- **Purpose**: Player character
- **Additional Features**:
  - Quest log
  - Skill trees
  - Achievements
  - Save/load

#### NPC (`src/game/entities/NPC.cpp`)
- **Purpose**: Non-player characters
- **Features**:
  - Dialogue trees
  - Quest givers
  - Merchants
  - Companions

#### Enemy (`src/game/entities/Enemy.cpp`)
- **Purpose**: Enemy characters
- **Features**:
  - AI behavior
  - Loot tables
  - Spawn rules
  - Difficulty scaling

### World System

#### World (`src/game/world/World.cpp`)
- **Purpose**: World container and manager
- **Responsibilities**:
  - Location management
  - World persistence
  - Time/weather systems
  - Event scheduling

#### Location (`src/game/world/Location.cpp`)
- **Purpose**: Large area (town, dungeon, forest)
- **Contains**:
  - Multiple rooms
  - NPCs
  - Ambient description
  - Music theme

#### Room (`src/game/world/Room.cpp`)
- **Purpose**: Individual scene/room
- **Properties**:
  - Description
  - Exits (connections to other rooms)
  - Items
  - NPCs
  - Events/triggers

### Combat System

#### CombatSystem (`src/game/combat/CombatSystem.cpp`)
- **Type**: Turn-based combat
- **Turn Order**: Speed-based initiative
- **Actions**:
  - Basic attack
  - Use ability
  - Use item
  - Defend
  - Flee

#### Ability (`src/game/combat/Ability.cpp`)
- **Types**:
  - Damage abilities
  - Healing abilities
  - Buff/debuff abilities
  - AOE abilities
- **Properties**:
  - Cost (MP)
  - Cooldown
  - Target type
  - Effects

#### StatusEffect (`src/game/combat/StatusEffect.cpp`)
- **Types**:
  - Buffs (increased stats)
  - Debuffs (decreased stats)
  - DOT (damage over time)
  - HOT (heal over time)
  - Stun/silence
- **Properties**:
  - Duration
  - Stacking rules
  - Application effects

### Item System

#### Item (`src/game/items/Item.cpp`)
- **Base Properties**:
  - Name, description
  - Icon/sprite ID
  - Weight, value
  - Rarity
  - Stack size

#### Inventory (`src/game/items/Inventory.cpp`)
- **Features**:
  - Slot-based or weight-based
  - Item stacking
  - Sorting and filtering
  - Quest item handling

#### Equipment (`src/game/items/Equipment.cpp`)
- **Slots**:
  - Main hand
  - Off hand
  - Head, chest, legs, feet
  - Accessories
- **Features**:
  - Stat bonuses
  - Set bonuses
  - Requirements (level, stats)

### Quest System

#### Quest (`src/game/quest/Quest.cpp`)
- **Components**:
  - Title and description
  - Objectives
  - Rewards
  - Prerequisites
  - Time limits

#### QuestManager (`src/game/quest/QuestManager.cpp`)
- **Responsibilities**:
  - Quest tracking
  - Progress updates
  - Completion detection
  - Reward distribution

#### Objective (`src/game/quest/Objective.cpp`)
- **Types**:
  - Kill objectives
  - Collect objectives
  - Talk to NPC
  - Reach location
  - Escort objectives

### Dialogue System

#### DialogueSystem (`src/game/dialogue/DialogueSystem.cpp`)
- **Purpose**: Manage dialogue state and flow
- **Features**:
  - Branching conversations
  - Conditional responses
  - Quest integration
  - Reputation effects

#### DialogueTree (`src/game/dialogue/DialogueTree.cpp`)
- **Structure**: Tree of dialogue nodes
- **Features**:
  - Multiple choice responses
  - Conditions (quest state, items, stats)
  - Actions (give item, start quest)

#### DialogueNode (`src/game/dialogue/DialogueNode.cpp`)
- **Properties**:
  - Speaker
  - Text
  - Choices
  - Conditions
  - Actions

## Data Flow

### Game Loop Data Flow

```
Input Events
    │
    ▼
InputManager ──┐
               │
               ▼
          CommandParser
               │
               ▼
          GameState ──────┐
               │          │
               ▼          ▼
         Entity System  World System
               │          │
               └──┬───────┘
                  │
                  ▼
            Combat System
            Quest System
            Dialogue System
                  │
                  ▼
              Renderer ◄─── AnimationSystem
                  │
                  ▼
              Display
```

### Resource Loading Flow

```
Asset Request
    │
    ▼
ResourceManager
    │
    ├─ Cache Hit? ──Yes──► Return Resource
    │
    └─ No
       │
       ▼
  File System
       │
       ▼
  Decode/Process
       │
       ▼
  Store in Cache
       │
       ▼
  Return Resource
```

## Threading Model

The engine currently uses a single-threaded model for simplicity:

- Main thread handles all game logic, rendering, and audio
- Async I/O for file loading (optional)
- Background music streaming on separate thread

Future improvements could include:
- Worker thread pool for asset loading
- Separate render thread
- Physics/AI thread

## Memory Management

### Resource Lifetime

- **Engine Resources**: Lifetime managed by `ResourceManager`
- **Game Entities**: Owned by `World` or `GameState`
- **UI Widgets**: Owned by `UIManager` in widget tree

### Smart Pointers Usage

```cpp
// Unique ownership
std::unique_ptr<Entity> entity;

// Shared ownership
std::shared_ptr<Texture> texture;

// Weak references (avoid circular dependencies)
std::weak_ptr<Quest> relatedQuest;
```

## Error Handling

### Strategy

1. **Critical Errors**: Throw exceptions for unrecoverable errors
2. **Recoverable Errors**: Return error codes or `std::optional`
3. **Logging**: Log all errors with context
4. **User Feedback**: Display user-friendly error messages

### Example

```cpp
std::optional<Shader> loadShader(const std::string& path)
{
    try
    {
        // Load and compile shader
        return Shader{path};
    }
    catch (const std::exception& e)
    {
        Logger::error("Failed to load shader {}: {}", path, e.what());
        return std::nullopt;
    }
}
```

## Performance Considerations

### Optimizations

1. **Render Batching**: Group similar render calls
2. **Object Pooling**: Reuse entities and particles
3. **Spatial Partitioning**: Quadtree for entity queries
4. **Lazy Loading**: Load resources on demand
5. **Asset Streaming**: Stream large audio files

### Profiling Points

- Frame time
- Render time
- Update time
- Memory usage
- Asset loading time

## Testing Strategy

### Unit Tests
- Core utilities (Timer, Config, Logger)
- Game logic (Combat calculations, Quest tracking)
- Data structures (Inventory, Dialogue trees)

### Integration Tests
- System interactions (Input → Command → Game State)
- Save/load functionality
- Combat flow

### Manual Testing
- Gameplay scenarios
- UI/UX validation
- Performance testing

## Future Extensions

### Planned Features
- Modding API
- Scripting language (Lua)
- Multiplayer support
- Mobile platform support

### Extensibility Points
- Plugin system for custom game modes
- Data-driven content creation
- Custom shader effects
- Procedural generation hooks

## References

- [SOLID Principles](https://en.wikipedia.org/wiki/SOLID)
- [Game Programming Patterns](https://gameprogrammingpatterns.com/)
- [GNU Coding Standards](https://www.gnu.org/prep/standards/)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
