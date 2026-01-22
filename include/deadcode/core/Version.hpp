/**
 * @file Version.hpp
 * @brief Version information for 0xDEADC0DE
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-22
 */

#pragma once

#include "deadcode/core/Types.hpp"

#include <string>

namespace deadcode
{

/**
 * @brief Version information namespace
 *
 * Contains version numbers and utility functions for version display.
 */
namespace Version
{

// Version numbers (automatically synced with CMakeLists.txt)
constexpr int32 MAJOR = 0;
constexpr int32 MINOR = 1;
constexpr int32 PATCH = 0;

// Build information
constexpr const char* BUILD_TYPE =
#ifdef NDEBUG
    "Release";
#else
    "Debug";
#endif

// Version string format: "v0.1.0"
inline String
getVersionString()
{
    return "v" + std::to_string(MAJOR) + "." + std::to_string(MINOR) + "." + std::to_string(PATCH);
}

// Full version with build type: "v0.1.0 (Release)"
inline String
getFullVersionString()
{
    return getVersionString() + " (" + BUILD_TYPE + ")";
}

// Game title with version: "0xDEADC0DE v0.1.0"
inline String
getGameTitleWithVersion()
{
    return "0xDEADC0DE " + getVersionString();
}

}  // namespace Version

}  // namespace deadcode
