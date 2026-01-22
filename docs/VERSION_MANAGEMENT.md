# Version Management

## Current Version: 0.1.0

## Version System

The project uses a centralized version management system defined in `include/deadcode/core/Version.hpp`.

### Version Format

The version follows semantic versioning: `MAJOR.MINOR.PATCH`

- **MAJOR**: Incompatible API changes
- **MINOR**: Backward-compatible functionality additions
- **PATCH**: Backward-compatible bug fixes

### Version Files

1. **VERSION** (root): Single-line text file with current version (e.g., `0.1.0`)
2. **Version.hpp**: C++ header with version constants and utility functions
3. **CMakeLists.txt**: Project version at line 21

### Updating Version

To update the version, modify these files:

1. Update `VERSION` file:
   ```
   0.2.0
   ```

2. Update `include/deadcode/core/Version.hpp`:
   ```cpp
   constexpr int32 MAJOR = 0;
   constexpr int32 MINOR = 2;
   constexpr int32 PATCH = 0;
   ```

3. Update `CMakeLists.txt`:
   ```cmake
   project(DeadCodeRPG
     VERSION 0.2.0
     ...
   )
   ```

### Version Display

The version is displayed in multiple places:

1. **Window Title**: "0xDEADC0DE v0.1.0 - Text-Based RPG"
2. **Main Menu**: Title shows "0xDEADC0DE" with version "v0.1.0" below
3. **Startup Logs**: Shows version and build type

### Version API

Available functions in `deadcode::Version` namespace:

```cpp
// Get version string: "v0.1.0"
String version = Version::getVersionString();

// Get full version with build type: "v0.1.0 (Release)"
String fullVersion = Version::getFullVersionString();

// Get game title with version: "0xDEADC0DE v0.1.0"
String titleWithVersion = Version::getGameTitleWithVersion();

// Access individual components
int32 major = Version::MAJOR;  // 0
int32 minor = Version::MINOR;  // 1
int32 patch = Version::PATCH;  // 0

// Get build type
const char* buildType = Version::BUILD_TYPE;  // "Release" or "Debug"
```

### Example Usage

```cpp
#include "deadcode/core/Version.hpp"

// Display version in logs
Logger::info("Game Version: {}", Version::getVersionString());

// Set window title with version
windowTitle = Version::getGameTitleWithVersion();

// Check version for compatibility
if (Version::MAJOR == 0) {
    Logger::warn("Development version - features may be incomplete");
}
```

## Version History

### 0.1.0 (2026-01-22)
- Initial release
- Main menu system with keyboard and mouse controls
- Basic game state management
- Window resize support
- Version management system
