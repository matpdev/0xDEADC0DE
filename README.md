# 0xDEADC0DE - Text-Based RPG

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.21+-064F8C.svg)](https://cmake.org/)

A modern text-based RPG with stunning OpenGL-powered text animations, combining classic command-line gameplay with visual effects.

## Features

- 🎮 **Classic Text-Based Gameplay**: Navigate through a rich fantasy world using text commands
- ✨ **OpenGL Animations**: Beautiful text rendering with smooth animations and visual effects
- ⚔️ **Turn-Based Combat**: Strategic combat system with abilities and status effects
- 📦 **Inventory System**: Manage weapons, armor, and consumables
- 🗺️ **Open World**: Explore interconnected locations and discover secrets
- 💬 **Dynamic Dialogues**: Branching conversation trees with NPCs
- 🎯 **Quest System**: Main storyline and side quests
- 💾 **Save System**: JSON-based save files
- 🎵 **Immersive Audio**: Sound effects and background music

## Screenshots

```
╔═══════════════════════════════════════════════════════════════════════════════╗
║                          The Dark Forest                                      ║
╠═══════════════════════════════════════════════════════════════════════════════╣
║  You find yourself in a dense forest. Ancient trees tower above you,         ║
║  their branches forming a canopy that blocks most of the sunlight.           ║
║  The air is thick with the smell of moss and decay.                          ║
║                                                                               ║
║  Exits: [north] [east] [south]                                               ║
║                                                                               ║
║  > _                                                                          ║
╚═══════════════════════════════════════════════════════════════════════════════╝
```

## Requirements

### System Requirements
- **OS**: Linux, Windows, or macOS
- **Compiler**: GCC 10+, Clang 12+, or MSVC 2019+
- **CMake**: 3.21 or higher
- **vcpkg**: Latest version
- **Graphics**: OpenGL 3.3+ compatible GPU

### Dependencies
All dependencies are managed via vcpkg:
- **GLFW** 3.3.8+ - Window and input management
- **GLEW** 2.2.0+ - OpenGL extension loading
- **GLM** 0.9.9+ - Mathematics library
- **FreeType** 2.12.0+ - Font rendering
- **spdlog** 1.11.0+ - Logging
- **nlohmann-json** 3.11.0+ - JSON parsing
- **OpenAL Soft** 1.23.0+ - Audio engine
- **libvorbis** 1.3.7+ - Audio codec
- **stb** (header-only) - Image and font loading

## Building from Source

### 1. Install vcpkg

```bash
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh  # or bootstrap-vcpkg.bat on Windows

# Set environment variable
export VCPKG_ROOT=/path/to/vcpkg
```

### 2. Clone the Repository

```bash
git clone https://github.com/yourusername/0xDEADC0DE.git
cd 0xDEADC0DE
```

### 3. Build

#### Using the Build Script (Recommended)

```bash
chmod +x scripts/build.sh
./scripts/build.sh release
```

#### Manual Build

```bash
# Configure
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release -j$(nproc)

# Run
./build/bin/deadcode_rpg
```

### Build Options

```bash
# Build with tests
cmake -B build -S . -DBUILD_TESTS=ON

# Build with documentation
cmake -B build -S . -DBUILD_DOCS=ON

# Enable sanitizers (for development)
cmake -B build -S . -DENABLE_SANITIZERS=ON -DCMAKE_BUILD_TYPE=Debug

# Enable static analysis
cmake -B build -S . -DENABLE_CLANG_TIDY=ON -DENABLE_CPPCHECK=ON
```

## Quick Start

### Running the Game

```bash
# From build directory
./build/bin/deadcode_rpg

# Or using the run script
./scripts/run.sh
```

### Basic Commands

```
Movement:
  go <direction>     - Move in a direction (north, south, east, west)
  look               - Examine your surroundings
  look at <object>   - Examine an object

Inventory:
  inventory          - Show your inventory
  equip <item>       - Equip an item
  use <item>         - Use a consumable item
  drop <item>        - Drop an item

Combat:
  attack             - Basic attack
  use <ability>      - Use an ability
  defend             - Defensive stance
  flee               - Attempt to escape

Interaction:
  talk to <npc>      - Start a conversation
  take <item>        - Pick up an item
  open <container>   - Open a container

System:
  save               - Save the game
  load               - Load a saved game
  quit               - Exit the game
  help               - Show all commands
```

## Development

### Project Structure

```
0xDEADC0DE/
├── assets/           # Game resources
│   ├── config/       # Configuration files
│   ├── data/         # Game data (JSON)
│   ├── fonts/        # TrueType fonts
│   ├── music/        # Background music
│   ├── shaders/      # GLSL shaders
│   └── sounds/       # Sound effects
├── build/            # Build output
├── cmake/            # CMake modules
├── docs/             # Documentation
├── external/         # Third-party code
├── include/          # Public headers
│   └── deadcode/     # Project namespace
├── scripts/          # Utility scripts
├── src/              # Source implementation
│   ├── audio/        # Audio system
│   ├── core/         # Core engine
│   ├── game/         # Game logic
│   ├── graphics/     # Rendering
│   ├── input/        # Input handling
│   └── ui/           # User interface
└── tests/            # Unit tests
```

### Code Style

The project follows GNU coding standards with modern C++20 adaptations. Code formatting is enforced via clang-format:

```bash
# Format all code
./scripts/format.sh

# Check formatting
./scripts/format.sh --check
```

### Testing

```bash
# Build and run tests
cmake -B build -S . -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build

# Or using the test script
./scripts/test.sh
```

### Contributing

We welcome contributions! Please read [CONTRIBUTING.md](docs/CONTRIBUTING.md) for guidelines.

## Architecture

The project is organized into three main components:

1. **Engine Library** (`deadcode_engine`): Core systems (graphics, audio, input)
2. **Game Library** (`deadcode_game`): Game-specific logic (entities, combat, quests)
3. **Executable** (`deadcode_rpg`): Main application entry point

For detailed architecture information, see [ARCHITECTURE.md](docs/ARCHITECTURE.md).

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## Credits

### Development Team
- **Lead Developer**: [Your Name]

### Third-Party Libraries
- [GLFW](https://www.glfw.org/) - Window and input management
- [GLEW](http://glew.sourceforge.net/) - OpenGL extension loading
- [GLM](https://github.com/g-truc/glm) - Mathematics library
- [FreeType](https://www.freetype.org/) - Font rendering
- [spdlog](https://github.com/gabime/spdlog) - Fast logging library
- [JSON for Modern C++](https://github.com/nlohmann/json) - JSON parsing
- [OpenAL Soft](https://github.com/kcat/openal-soft) - Audio engine
- [libvorbis](https://xiph.org/vorbis/) - Audio codec
- [stb](https://github.com/nothings/stb) - Image and font loading

### Assets
- Fonts: [List font sources and licenses]
- Sound effects: [List sound sources and licenses]
- Music: [List music sources and licenses]

## Support

- **Issues**: [GitHub Issues](https://github.com/yourusername/0xDEADC0DE/issues)
- **Discussions**: [GitHub Discussions](https://github.com/yourusername/0xDEADC0DE/discussions)
- **Wiki**: [Project Wiki](https://github.com/yourusername/0xDEADC0DE/wiki)

## Roadmap

### Version 0.1.0 (Current)
- [x] Core engine implementation
- [x] Basic game loop
- [x] Text rendering with animations
- [x] Command system
- [ ] Initial quest line

### Version 0.2.0 (Planned)
- [ ] Enhanced combat system
- [ ] More locations and quests
- [ ] Character customization
- [ ] Achievement system

### Version 1.0.0 (Future)
- [ ] Complete main storyline
- [ ] Modding support
- [ ] Multiplayer features

## Acknowledgments

Special thanks to all contributors and the open-source community for their invaluable tools and libraries.

---

**Built with ❤️ and C++20**
