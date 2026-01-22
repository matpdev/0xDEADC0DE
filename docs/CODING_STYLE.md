# Coding Style Guide - 0xDEADC0DE

This document outlines the coding style and conventions for the 0xDEADC0DE project. We follow GNU coding standards with modern C++20 adaptations.

## Table of Contents
1. [General Principles](#general-principles)
2. [File Organization](#file-organization)
3. [Naming Conventions](#naming-conventions)
4. [Formatting](#formatting)
5. [Comments and Documentation](#comments-and-documentation)
6. [C++ Best Practices](#c-best-practices)
7. [Error Handling](#error-handling)

## General Principles

### Code Quality Goals
- **Readability**: Code should be self-documenting and easy to understand
- **Maintainability**: Changes should be localized and predictable
- **Performance**: Optimize only when necessary, profile first
- **Safety**: Prefer compile-time checks over runtime checks

### Modern C++20 Features
We embrace modern C++ features:
- Concepts and constraints
- Ranges library
- Coroutines (where appropriate)
- Three-way comparison operator
- Designated initializers
- `std::format` (or fmt library)

## File Organization

### Directory Structure
```
include/deadcode/    # Public headers
src/                 # Implementation files
tests/               # Test files
```

### Header Files (.hpp)

```cpp
#pragma once

// 1. Related header (for .cpp files)
// 2. C system headers
#include <cstdint>
#include <cstring>

// 3. C++ standard library headers
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

// 4. Third-party library headers
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

// 5. Project headers (alphabetically)
#include "deadcode/core/Logger.hpp"
#include "deadcode/core/Types.hpp"

namespace deadcode
{

// Class declarations, function declarations, etc.

}  // namespace deadcode
```

### Source Files (.cpp)

```cpp
// 1. Corresponding header
#include "deadcode/ClassName.hpp"

// 2. Other headers (same order as in header files)
#include <iostream>
#include <stdexcept>

#include "deadcode/core/Logger.hpp"

namespace deadcode
{

// Implementation

}  // namespace deadcode
```

### Include Guards

**Preferred**: Use `#pragma once`

```cpp
#pragma once
```

**Alternative**: Traditional include guards (for maximum compatibility)

```cpp
#ifndef DEADCODE_CORE_APPLICATION_HPP
#define DEADCODE_CORE_APPLICATION_HPP

// Content

#endif  // DEADCODE_CORE_APPLICATION_HPP
```

## Naming Conventions

### Overview Table

| Element | Convention | Example |
|---------|-----------|---------|
| Classes/Structs | PascalCase | `TextRenderer`, `GameState` |
| Functions/Methods | camelCase | `calculateDamage()`, `update()` |
| Variables | camelCase | `playerHealth`, `itemCount` |
| Private Members | m_ prefix + camelCase | `m_health`, `m_inventory` |
| Protected Members | m_ prefix + camelCase | `m_baseValue` |
| Constants | UPPER_CASE | `MAX_SIZE`, `PI` |
| Enums | PascalCase | `State`, `ErrorCode` |
| Enum Values | UPPER_CASE | `IDLE`, `ACTIVE`, `ERROR_NONE` |
| Namespaces | lower_case | `deadcode`, `game_logic` |
| Template Parameters | PascalCase | `T`, `Container`, `ValueType` |
| Macros | UPPER_CASE | `DEADCODE_VERSION` |

### Detailed Examples

#### Classes and Structs

```cpp
class TextRenderer  // PascalCase
{
};

struct PlayerStats  // PascalCase
{
};
```

#### Functions and Methods

```cpp
// Free functions
void initializeEngine();
int calculateDamage(int attack, int defense);

// Methods
class Character
{
  public:
    void takeDamage(int amount);
    [[nodiscard]] int getHealth() const;
    void setHealth(int value);
};
```

#### Variables

```cpp
// Local variables
int playerHealth = 100;
std::string itemName = "Sword";
float damageMultiplier = 1.5f;

// Function parameters
void processItem(const std::string& itemName, int quantity);
```

#### Member Variables

```cpp
class Character
{
  private:
    // Private members: m_ prefix
    int m_health;
    int m_maxHealth;
    std::string m_name;
    std::vector<Item> m_inventory;

  protected:
    // Protected members: m_ prefix
    float m_baseSpeed;
    int m_level;
};
```

#### Constants

```cpp
// Global constants
constexpr int MAX_INVENTORY_SIZE = 100;
constexpr float PI = 3.14159f;
const std::string DEFAULT_PLAYER_NAME = "Hero";

// Class constants
class Config
{
  public:
    static constexpr int WINDOW_WIDTH = 1920;
    static constexpr int WINDOW_HEIGHT = 1080;
};
```

#### Enumerations

```cpp
// Enum class (preferred)
enum class State
{
    IDLE,
    ACTIVE,
    PAUSED,
    TERMINATED
};

// Traditional enum (for bit flags)
enum ErrorCode
{
    ERROR_NONE = 0,
    ERROR_FILE_NOT_FOUND = 1,
    ERROR_INVALID_FORMAT = 2
};
```

#### Namespaces

```cpp
namespace deadcode
{
namespace game_logic  // Nested namespace with snake_case
{

class CombatSystem
{
};

}  // namespace game_logic
}  // namespace deadcode

// C++17 nested namespace syntax (preferred)
namespace deadcode::game_logic
{

class CombatSystem
{
};

}  // namespace deadcode::game_logic
```

#### Template Parameters

```cpp
template<typename T>  // Single letter for simple templates
class Container
{
};

template<typename KeyType, typename ValueType>  // Descriptive for complex templates
class Map
{
};
```

## Formatting

### Indentation

- **Use 4 spaces** for indentation (no tabs)
- Continuation lines: 4 spaces
- Access modifiers: Outdented by 4 spaces

```cpp
class Example
{
  public:  // Outdented
    void method()
    {
        if (condition)
        {
            doSomething();
        }
    }

  private:
    int m_value;
};
```

### Braces

**GNU Style**: Opening brace on new line

```cpp
// Functions
void functionName()
{
    // body
}

// Classes
class ClassName
{
  public:
    // members
};

// Control structures
if (condition)
{
    doSomething();
}
else
{
    doSomethingElse();
}

// Loops
for (int i = 0; i < count; ++i)
{
    process(i);
}

while (running)
{
    update();
}

// Switch statements
switch (value)
{
    case 1:
    {
        handleCase1();
        break;
    }
    case 2:
    {
        handleCase2();
        break;
    }
    default:
    {
        handleDefault();
        break;
    }
}
```

**Exception**: Empty functions and trivial lambdas

```cpp
// Empty functions
void emptyFunction() { }

// Trivial getters
[[nodiscard]] int getValue() const { return m_value; }

// Simple lambdas
auto lambda = [](int x) { return x * 2; };
```

### Line Length

- **Maximum**: 100 characters
- Break long lines intelligently
- Align continuation lines

```cpp
// Good: Function with many parameters
void longFunctionName(int firstParameter,
                      int secondParameter,
                      int thirdParameter,
                      const std::string& fourthParameter);

// Good: Long expression
int result = calculateSomething(firstValue, secondValue)
             + calculateSomethingElse(thirdValue, fourthValue)
             - adjustment;

// Good: Long string
const char* message = "This is a very long message that needs to be "
                      "split across multiple lines for readability.";
```

### Spacing

#### Around Operators

```cpp
// Binary operators: spaces on both sides
int sum = a + b;
int product = x * y;
bool result = a && b || c;

// Unary operators: no space
int negative = -value;
int incremented = ++counter;
bool notFlag = !isActive;

// Pointer/reference: attached to type
int* pointer;
int& reference;
const std::string& str;

// Member access: no spaces
object.member;
pointer->member;
```

#### After Keywords

```cpp
// Space after keywords
if (condition)
while (running)
for (int i = 0; i < n; ++i)
switch (value)

// No space for function-like keywords
sizeof(int)
alignof(double)
decltype(variable)
```

#### Function Calls

```cpp
// No space before parenthesis
functionName(arg1, arg2);

// Space after comma
doSomething(first, second, third);

// No spaces inside parentheses
calculate(a + b, c * d);
```

### Blank Lines

```cpp
#include <iostream>
#include <string>
                              // Blank line after includes
namespace deadcode
{
                              // Blank line after namespace opening
class Example
{
  public:
    Example();
                              // Blank line between sections
    void method1();
    void method2();
                              // Blank line between method groups
  private:
    void helperMethod();
                              // Blank line before member variables
    int m_value;
    std::string m_name;
};
                              // Blank line before namespace closing
}  // namespace deadcode
```

## Comments and Documentation

### Doxygen Style

Use Javadoc-style comments for documentation:

```cpp
/**
 * @brief Brief description of the class.
 *
 * Detailed description of the class, its purpose,
 * and how it should be used.
 */
class ExampleClass
{
  public:
    /**
     * @brief Brief description of the method.
     *
     * Detailed description of what the method does.
     *
     * @param paramName Description of the parameter
     * @param anotherParam Description of another parameter
     * @return Description of the return value
     *
     * @throws std::runtime_error If something goes wrong
     *
     * @note Important note about usage
     * @warning Warning about potential issues
     * @see relatedFunction()
     *
     * @code
     * // Example usage
     * ExampleClass obj;
     * obj.method(42, "test");
     * @endcode
     */
    int method(int paramName, const std::string& anotherParam);
};
```

### Inline Comments

```cpp
// Good: Explain why, not what
// Use binary search because the array is already sorted
int index = binarySearch(array, value);

// Bad: Obvious comment
// Increment counter by one
counter++;  // Don't do this

// Good: Clarify complex logic
// Handle the special case where the user is in combat
// and trying to use a non-combat item
if (isInCombat() && !item->isUsableInCombat())
{
    showError("Cannot use this item during combat");
    return;
}
```

### TODO Comments

```cpp
// TODO(username): Brief description of what needs to be done
// TODO: Add error handling for network failures

// FIXME(username): Description of the bug
// FIXME: Memory leak in resource cleanup

// HACK: Explanation of why this is a hack
// HACK: Workaround for compiler bug in GCC 10

// NOTE: Important information
// NOTE: This function is called from multiple threads
```

### File Headers

```cpp
/**
 * @file FileName.hpp
 * @brief Brief description of file contents
 *
 * Detailed description of the file's purpose,
 * the main classes it contains, etc.
 *
 * @author Your Name
 * @date 2026-01-21
 */
```

## C++ Best Practices

### Modern C++ Features

#### Auto Keyword

```cpp
// Good: Obvious type
auto value = 42;
auto name = std::string{"Player"};
auto vec = std::vector<int>{1, 2, 3};

// Good: Complex iterator types
for (auto it = container.begin(); it != container.end(); ++it)

// Good: Lambdas
auto lambda = [](int x) { return x * 2; };

// Avoid: When type is not obvious
auto result = calculate();  // What type is this?

// Better: Explicit type for clarity
int result = calculate();
```

#### Range-Based For Loops

```cpp
// Good: Read-only access
for (const auto& item : inventory)
{
    processItem(item);
}

// Good: Modification
for (auto& item : inventory)
{
    item.update();
}

// Good: Copies (for small types)
for (auto item : numbers)  // int, float, etc.
{
    sum += item;
}
```

#### Smart Pointers

```cpp
// Unique ownership
auto entity = std::make_unique<Entity>();

// Shared ownership
auto texture = std::make_shared<Texture>("texture.png");

// Weak reference (avoid circular dependencies)
std::weak_ptr<Quest> relatedQuest = questManager->getQuest(id);

// Raw pointers: only for non-owning references
void processEntity(Entity* entity);  // Does not own
```

#### Nullptr

```cpp
// Always use nullptr, never NULL or 0
Entity* entity = nullptr;

if (entity != nullptr)
{
    // ...
}
```

#### Override and Final

```cpp
class Base
{
  public:
    virtual void method();
    virtual void finalMethod();
};

class Derived : public Base
{
  public:
    void method() override;  // Always use override
    void finalMethod() final;  // Prevent further overriding
};
```

#### Constexpr

```cpp
// Compile-time constants
constexpr int MAX_SIZE = 100;
constexpr double PI = 3.14159;

// Compile-time functions
constexpr int square(int x)
{
    return x * x;
}

constexpr int result = square(5);  // Computed at compile time
```

#### Const Correctness

```cpp
class Character
{
  public:
    // Const member function: doesn't modify object
    [[nodiscard]] int getHealth() const { return m_health; }
    
    // Non-const: modifies object
    void setHealth(int value) { m_health = value; }

  private:
    int m_health;
};

// Const references for parameters
void processEntity(const Entity& entity);

// Const pointers
void render(const Shader* shader);
```

#### [[nodiscard]]

```cpp
// Use for functions where ignoring the return value is a mistake
[[nodiscard]] bool saveGame(const std::string& filename);
[[nodiscard]] std::optional<Item> findItem(int id);
[[nodiscard]] int calculate() const;

// Usage
if (saveGame("save.json"))
{
    std::cout << "Game saved successfully\n";
}
```

### RAII (Resource Acquisition Is Initialization)

```cpp
// Good: RAII with smart pointers
{
    auto file = std::make_unique<File>("data.txt");
    file->read();
    // File automatically closed when unique_ptr is destroyed
}

// Good: Custom RAII wrapper
class ScopedTimer
{
  public:
    ScopedTimer(const std::string& name) : m_name(name), m_start(now()) {}
    ~ScopedTimer() { logDuration(m_name, now() - m_start); }

  private:
    std::string m_name;
    TimePoint m_start;
};

// Usage
{
    ScopedTimer timer("Combat Update");
    updateCombat();
    // Duration automatically logged when timer is destroyed
}
```

### Avoid Common Pitfalls

```cpp
// Avoid: Signed/unsigned comparisons
int count = getCount();
for (int i = 0; i < vec.size(); ++i)  // Warning: comparing signed and unsigned

// Better: Use size_t or auto
for (size_t i = 0; i < vec.size(); ++i)
for (auto i = 0u; i < vec.size(); ++i)

// Avoid: Implicit bool conversions
if (pointer)  // OK for pointers
if (count)    // Avoid for integers

// Better: Explicit comparisons
if (pointer != nullptr)
if (count > 0)

// Avoid: Magic numbers
const int maxHealth = 100;  // What does 100 mean?

// Better: Named constants
constexpr int DEFAULT_MAX_HEALTH = 100;
constexpr int WARRIOR_MAX_HEALTH = 150;
```

## Error Handling

### Exceptions

```cpp
// Throw exceptions for exceptional conditions
void loadResource(const std::string& path)
{
    if (!fileExists(path))
    {
        throw std::runtime_error("File not found: " + path);
    }
    // Load resource
}

// Catch by const reference
try
{
    loadResource("data.json");
}
catch (const std::exception& e)
{
    Logger::error("Failed to load resource: {}", e.what());
}
```

### Optional and Expected

```cpp
// Use std::optional for values that may not exist
std::optional<Item> findItem(int id)
{
    auto it = items.find(id);
    if (it != items.end())
    {
        return it->second;
    }
    return std::nullopt;
}

// Usage
if (auto item = findItem(42))
{
    useItem(*item);
}

// Consider std::expected (C++23) or custom result type
Result<Shader> loadShader(const std::string& path);
```

## Tool Configuration

The project uses automated formatting and linting:

### Clang-Format
Run before committing:
```bash
./scripts/format.sh
```

### Clang-Tidy
Check code quality:
```bash
./scripts/lint.sh
```

## References

- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [GNU Coding Standards](https://www.gnu.org/prep/standards/)
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [Modern C++ Features](https://en.cppreference.com/)
