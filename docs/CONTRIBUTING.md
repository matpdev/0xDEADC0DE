# Contributing to 0xDEADC0DE

Thank you for your interest in contributing to 0xDEADC0DE! This document provides guidelines and instructions for contributing to the project.

## Code of Conduct

By participating in this project, you agree to maintain a respectful and inclusive environment for all contributors.

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check existing issues to avoid duplicates. When creating a bug report, include:

- **Clear title and description**
- **Steps to reproduce** the issue
- **Expected behavior** vs actual behavior
- **Environment details** (OS, compiler version, CMake version)
- **Code samples or logs** if applicable

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. When suggesting an enhancement:

- **Use a clear and descriptive title**
- **Provide detailed description** of the suggested enhancement
- **Explain why** this enhancement would be useful
- **Provide examples** of how it would work

### Pull Requests

1. **Fork the repository** and create your branch from `main`
2. **Follow the coding style** (see below)
3. **Add tests** if applicable
4. **Update documentation** as needed
5. **Ensure all tests pass**
6. **Submit the pull request**

## Development Setup

### Prerequisites

```bash
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg && ./bootstrap-vcpkg.sh
export VCPKG_ROOT=$(pwd)

# Clone the repository
git clone https://github.com/yourusername/0xDEADC0DE.git
cd 0xDEADC0DE
```

### Building

```bash
# Configure and build
./scripts/build.sh debug

# Run tests
./scripts/test.sh
```

## Coding Standards

### C++ Style Guide

We follow GNU coding standards with C++20 modern features. Key points:

#### Naming Conventions

```cpp
// Classes and structs: PascalCase
class TextRenderer { };
struct GameState { };

// Functions and methods: camelCase
void updateGameState();
int calculateDamage(int base, float multiplier);

// Variables: camelCase
int playerHealth;
std::string itemName;

// Private/protected members: m_ prefix
class Character
{
  private:
    int m_health;
    std::string m_name;
};

// Constants: UPPER_CASE
constexpr int MAX_INVENTORY_SIZE = 100;
const float PI = 3.14159f;

// Namespaces: lower_case
namespace deadcode { }
namespace game_logic { }
```

#### Braces and Indentation

```cpp
// GNU-style braces (opening brace on new line)
void functionName()
{
    if (condition)
    {
        doSomething();
    }
    else
    {
        doSomethingElse();
    }
}

// Use 4 spaces for indentation (no tabs)
class Example
{
  public:
    void method()
    {
        int value = 42;
    }

  private:
    int m_data;
};
```

#### Modern C++ Features

```cpp
// Use auto for type deduction
auto value = calculateValue();
auto iterator = container.begin();

// Use range-based for loops
for (const auto& item : inventory)
{
    processItem(item);
}

// Use smart pointers
std::unique_ptr<Entity> entity = std::make_unique<Entity>();
std::shared_ptr<Resource> resource = std::make_shared<Resource>();

// Use nullptr instead of NULL
Widget* widget = nullptr;

// Use override and final keywords
class Derived : public Base
{
  public:
    void virtualMethod() override;
    void finalMethod() final;
};

// Use constexpr for compile-time constants
constexpr int MAX_SIZE = 100;
constexpr double calculateArea(double radius)
{
    return 3.14159 * radius * radius;
}
```

#### Header Guards

```cpp
// Use #pragma once (preferred)
#pragma once

// Or traditional include guards
#ifndef DEADCODE_CORE_APPLICATION_HPP
#define DEADCODE_CORE_APPLICATION_HPP
// ... content ...
#endif  // DEADCODE_CORE_APPLICATION_HPP
```

#### Documentation

Use Doxygen-style comments:

```cpp
/**
 * @brief Calculates damage based on attack and defense values.
 *
 * This function implements the damage calculation formula used
 * throughout the combat system.
 *
 * @param attackPower The attacker's power value
 * @param defenseValue The defender's defense value
 * @return The calculated damage amount
 *
 * @note Defense cannot reduce damage below 1
 * @see CombatSystem::applyDamage()
 */
int calculateDamage(int attackPower, int defenseValue);
```

### File Organization

#### Header Files (.hpp)

```cpp
#pragma once

// 1. System includes
#include <string>
#include <vector>

// 2. Third-party includes
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

// 3. Project includes
#include "deadcode/core/Types.hpp"

namespace deadcode
{

/**
 * @brief Brief class description.
 *
 * Detailed description of the class and its purpose.
 */
class ClassName
{
  public:
    // Public types and constants
    enum class State { IDLE, ACTIVE, PAUSED };

    // Constructors and destructor
    ClassName();
    explicit ClassName(int value);
    ~ClassName();

    // Delete copy constructor/assignment for non-copyable classes
    ClassName(const ClassName&) = delete;
    ClassName& operator=(const ClassName&) = delete;

    // Public methods
    void publicMethod();
    [[nodiscard]] int getValue() const;

  protected:
    // Protected methods
    void protectedMethod();

    // Protected members
    int m_protectedValue;

  private:
    // Private methods
    void privateMethod();

    // Private members
    int m_privateValue;
    std::string m_name;
};

}  // namespace deadcode
```

#### Source Files (.cpp)

```cpp
#include "deadcode/ClassName.hpp"

#include <algorithm>
#include <iostream>

namespace deadcode
{

ClassName::ClassName()
    : m_privateValue(0)
    , m_name("default")
{
    // Constructor implementation
}

void
ClassName::publicMethod()
{
    // Method implementation
}

}  // namespace deadcode
```

### Code Formatting

Always run clang-format before committing:

```bash
# Format all files
./scripts/format.sh

# Check formatting without modifying
./scripts/format.sh --check
```

### Static Analysis

Run static analysis tools:

```bash
# Run clang-tidy
./scripts/lint.sh

# Build with sanitizers
cmake -B build -S . -DENABLE_SANITIZERS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Testing

### Writing Tests

Tests use Google Test framework:

```cpp
#include <gtest/gtest.h>
#include "deadcode/core/Logger.hpp"

namespace deadcode::test
{

class LoggerTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Setup code
    }

    void TearDown() override
    {
        // Cleanup code
    }
};

TEST_F(LoggerTest, InitializesCorrectly)
{
    Logger logger;
    EXPECT_TRUE(logger.isInitialized());
}

TEST_F(LoggerTest, LogsMessages)
{
    Logger logger;
    ASSERT_NO_THROW(logger.info("Test message"));
}

}  // namespace deadcode::test
```

### Running Tests

```bash
# Build and run all tests
./scripts/test.sh

# Run specific test
./build/bin/deadcode_tests --gtest_filter=LoggerTest.*

# Run with verbose output
./build/bin/deadcode_tests --gtest_verbose
```

## Git Workflow

### Branching Strategy

- `main` - Stable release branch
- `develop` - Development branch
- `feature/*` - Feature branches
- `bugfix/*` - Bug fix branches
- `hotfix/*` - Urgent fixes for production

### Commit Messages

Follow conventional commits format:

```
<type>(<scope>): <subject>

<body>

<footer>
```

Types:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting)
- `refactor`: Code refactoring
- `test`: Adding or modifying tests
- `chore`: Maintenance tasks

Examples:

```
feat(combat): add status effect system

Implemented a flexible status effect system that supports
buffs, debuffs, and DOT effects.

Closes #42
```

```
fix(graphics): resolve text rendering flickering

Fixed issue where text would flicker during animations
caused by incorrect buffer synchronization.

Fixes #127
```

## Documentation

### Code Documentation

- Document all public APIs with Doxygen comments
- Explain complex algorithms and design decisions
- Keep comments up-to-date with code changes

### User Documentation

- Update README.md for user-facing changes
- Add examples for new features
- Update command reference for new commands

### Design Documentation

Document significant architectural decisions in `docs/design/`:

```markdown
# Feature Name

## Overview
Brief description of the feature

## Design
Detailed design explanation

## Alternatives Considered
What other approaches were considered and why they weren't chosen

## Implementation Details
Key implementation notes
```

## Performance Considerations

- Profile before optimizing
- Avoid premature optimization
- Document performance-critical sections
- Use appropriate data structures
- Consider cache locality

## Security Considerations

- Validate all user input
- Avoid buffer overflows
- Use safe string functions
- Don't store sensitive data in plain text
- Check bounds on array accesses

## Questions?

If you have questions, feel free to:
- Open a discussion on GitHub
- Ask in the issue tracker
- Contact the maintainers

Thank you for contributing to 0xDEADC0DE!
