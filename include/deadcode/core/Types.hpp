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
class Menu;

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
