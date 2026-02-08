/**
 * @file Types.hpp
 * @brief Common type definitions and aliases for the DeadCode RPG engine
 *
 * This file contains fundamental type definitions, aliases, and utility types
 * used throughout the engine.
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-21
 */

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <raylib.h>

namespace deadcode
{

// ============================================================================
// Integer Types
// ============================================================================

using int8   = std::int8_t;
using int16  = std::int16_t;
using int32  = std::int32_t;
using int64  = std::int64_t;
using uint8  = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

// ============================================================================
// Floating Point Types
// ============================================================================

using float32 = float;
using float64 = double;

// ============================================================================
// Common Aliases
// ============================================================================

using String     = std::string;
using StringView = std::string_view;
using ByteArray  = std::vector<uint8>;

// ============================================================================
// Smart Pointer Aliases
// ============================================================================

template <typename T>
using UniquePtr = std::unique_ptr<T>;

template <typename T>
using SharedPtr = std::shared_ptr<T>;

template <typename T>
using WeakPtr = std::weak_ptr<T>;

// ============================================================================
// Type Conversion Utilities (GLM <-> Raylib)
// ============================================================================

/**
 * @brief Convert glm::vec2 to Raylib Vector2
 */
inline Vector2
toRaylib(const glm::vec2& v)
{
    return Vector2{v.x, v.y};
}

/**
 * @brief Convert glm::vec3 (0-1) to Raylib Color
 */
inline Color
toRaylib(const glm::vec3& c)
{
    return Color{static_cast<uint8>(c.r * 255.0f), static_cast<uint8>(c.g * 255.0f),
                 static_cast<uint8>(c.b * 255.0f), 255};
}

/**
 * @brief Convert glm::vec4 (0-1) to Raylib Color
 */
inline Color
toRaylib(const glm::vec4& c)
{
    return Color{static_cast<uint8>(c.r * 255.0f), static_cast<uint8>(c.g * 255.0f),
                 static_cast<uint8>(c.b * 255.0f), static_cast<uint8>(c.a * 255.0f)};
}

/**
 * @brief Convert Raylib Vector2 to glm::vec2
 */
inline glm::vec2
toGLM(const Vector2& v)
{
    return glm::vec2{v.x, v.y};
}

/**
 * @brief Convert Raylib Color to glm::vec4 (0-1)
 */
inline glm::vec4
toGLM(const Color& c)
{
    return glm::vec4{c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f};
}

// ============================================================================
// Forward Declarations
// ============================================================================

class Application;
class Logger;
class Config;
class Timer;
class ResourceManager;

// Graphics
class Window;
class Renderer;
class Shader;
class TextRenderer;
class AnimationSystem;

// Input
class InputManager;
class TextInput;
class CommandParser;

// Audio
class AudioManager;
class SoundEffect;
class MusicPlayer;

// UI
class UIManager;
class Console;
class TextBox;
class MenuFrame;
class StartMenu;

// Game
// GameState is an enum class defined in deadcode/game/GameState.hpp
class GameLoop;
class SaveSystem;

// Entities
class Entity;
class Character;
class Player;
class NPC;
class Enemy;

// World
class World;
class Location;
class Room;

// Combat
class CombatSystem;
class Ability;
class StatusEffect;

// Items
class Item;
class Inventory;
class Equipment;

// Quest
class Quest;
class QuestManager;
class Objective;

// Dialogue
class DialogueSystem;
class DialogueTree;
class DialogueNode;

}  // namespace deadcode
