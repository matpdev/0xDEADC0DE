# 0xDEADC0DE - Text-Based RPG with OpenGL Animations
# CMake Build System Configuration
# Minimum version 3.21 for proper vcpkg and C++20 support

cmake_minimum_required(VERSION 3.21)

# -----------------------------------------------------------------------------
# vcpkg Toolchain Configuration (must be before project())
# -----------------------------------------------------------------------------
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "vcpkg toolchain file")
endif()

# Enable vcpkg manifest mode
set(VCPKG_MANIFEST_MODE ON)

# -----------------------------------------------------------------------------
# Project Definition
# -----------------------------------------------------------------------------
project(DeadCodeRPG
  VERSION 0.1.0
  DESCRIPTION "0xDEADC0DE - A text-based RPG with Raylib rendering"
  HOMEPAGE_URL "https://github.com/0xDEADC0DE/rpg"
  LANGUAGES CXX C
)

# -----------------------------------------------------------------------------
# Build Options
# -----------------------------------------------------------------------------
option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_DOCS "Build documentation" OFF)
option(ENABLE_SANITIZERS "Enable address and undefined sanitizers" OFF)
option(ENABLE_COVERAGE "Enable code coverage" OFF)
option(ENABLE_CLANG_TIDY "Enable clang-tidy analysis" OFF)
option(ENABLE_CPPCHECK "Enable cppcheck analysis" OFF)

# -----------------------------------------------------------------------------
# C++ Standard Configuration
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile_commands.json for IDEs and tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------------------------
# Build Type Configuration
# -----------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# -----------------------------------------------------------------------------
# Output Directories (GNU Style)
# -----------------------------------------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# -----------------------------------------------------------------------------
# Compiler Warnings (GNU Style)
# -----------------------------------------------------------------------------
add_library(project_warnings INTERFACE)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(project_warnings INTERFACE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2
    -Wimplicit-fallthrough
  )

  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(project_warnings INTERFACE
      -Wmisleading-indentation
      -Wduplicated-cond
      -Wduplicated-branches
      -Wlogical-op
      -Wuseless-cast
    )
  endif()
elseif(MSVC)
  target_compile_options(project_warnings INTERFACE
    /W4
    /permissive-
    /w14640
    /w14242
    /w14254
    /w14263
    /w14265
    /w14287
    /w14296
    /w14311
    /w14545
    /w14546
    /w14547
    /w14549
    /w14555
    /w14619
    /w14640
    /w14826
    /w14905
    /w14906
    /w14928
  )
endif()

# -----------------------------------------------------------------------------
# Project Options Library (for settings propagation)
# -----------------------------------------------------------------------------
add_library(project_options INTERFACE)

# Sanitizers
if(ENABLE_SANITIZERS)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(project_options INTERFACE
      -fsanitize=address,undefined
      -fno-omit-frame-pointer
    )
    target_link_options(project_options INTERFACE
      -fsanitize=address,undefined
    )
  endif()
endif()

# Coverage
if(ENABLE_COVERAGE)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(project_options INTERFACE --coverage)
    target_link_options(project_options INTERFACE --coverage)
  endif()
endif()

# -----------------------------------------------------------------------------
# Find Dependencies (via vcpkg)
# -----------------------------------------------------------------------------
find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)  # Raylib depends on GLFW
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json 3.11.0 CONFIG REQUIRED)
find_package(Tweeny CONFIG REQUIRED)

# stb is header-only, handled in include paths

# -----------------------------------------------------------------------------
# Static Analysis Tools
# -----------------------------------------------------------------------------
if(ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES clang-tidy)
  if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
  else()
    message(WARNING "clang-tidy not found!")
  endif()
endif()

if(ENABLE_CPPCHECK)
  find_program(CPPCHECK_EXE NAMES cppcheck)
  if(CPPCHECK_EXE)
    set(CMAKE_CXX_CPPCHECK
      ${CPPCHECK_EXE}
      --enable=all
      --suppress=missingIncludeSystem
      --inline-suppr
    )
    message(STATUS "cppcheck found: ${CPPCHECK_EXE}")
  else()
    message(WARNING "cppcheck not found!")
  endif()
endif()

# -----------------------------------------------------------------------------
# Project Include Directories
# -----------------------------------------------------------------------------
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PROJECT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(PROJECT_ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)

# -----------------------------------------------------------------------------
# Main Engine Library
# -----------------------------------------------------------------------------
add_library(deadcode_engine STATIC
  # Core
  src/core/Application.cpp
  src/core/Logger.cpp
  src/core/Config.cpp
  src/core/Timer.cpp
  src/core/ResourceManager.cpp

  # Game Systems (SaveSystem needed by Application)
  src/game/SaveSystem.cpp
  src/game/GameLoop.cpp

  # Graphics
  src/graphics/Window.cpp
  src/graphics/Renderer.cpp
  src/graphics/TextRenderer.cpp
  src/graphics/AnimationSystem.cpp
  src/graphics/GlitchEffect.cpp
  src/graphics/EffectManager.cpp

  # Input
  src/input/InputManager.cpp
  src/input/TextInput.cpp
  src/input/CommandParser.cpp

  # Audio
  src/audio/AudioManager.cpp
  src/audio/SoundEffect.cpp
  src/audio/MusicPlayer.cpp

  # UI
  src/ui/UIManager.cpp
  src/ui/Console.cpp
  src/ui/TextBox.cpp
  src/ui/MenuFrame.cpp
  src/ui/StartMenu.cpp
  src/ui/ConfigMenu.cpp
)

target_include_directories(deadcode_engine
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(deadcode_engine
  PUBLIC
    raylib
    glfw  # Required by Raylib
    glm::glm
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    tweeny
    $<BUILD_INTERFACE:project_options>
    $<BUILD_INTERFACE:project_warnings>
)

# Set target properties
set_target_properties(deadcode_engine PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  POSITION_INDEPENDENT_CODE ON
)

# -----------------------------------------------------------------------------
# Game Library
# -----------------------------------------------------------------------------
add_library(deadcode_game STATIC
  # Game Systems

  # Entities
  src/game/entities/Entity.cpp
  src/game/entities/Character.cpp
  src/game/entities/Player.cpp
  src/game/entities/NPC.cpp
  src/game/entities/Enemy.cpp

  # World
  src/game/world/World.cpp
  src/game/world/Location.cpp
  src/game/world/Room.cpp

  # Combat
  src/game/combat/CombatSystem.cpp
  src/game/combat/Ability.cpp
  src/game/combat/StatusEffect.cpp

  # Items
  src/game/items/Item.cpp
  src/game/items/Inventory.cpp
  src/game/items/Equipment.cpp

  # Quest
  src/game/quest/Quest.cpp
  src/game/quest/QuestManager.cpp
  src/game/quest/Objective.cpp

  # Dialogue
  src/game/dialogue/DialogueSystem.cpp
  src/game/dialogue/DialogueTree.cpp
  src/game/dialogue/DialogueNode.cpp
)

target_include_directories(deadcode_game
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(deadcode_game
  PUBLIC
    deadcode_engine
)

# -----------------------------------------------------------------------------
# Main Executable
# -----------------------------------------------------------------------------
add_executable(deadcode_rpg
  src/main.cpp
)

target_link_libraries(deadcode_rpg
  PRIVATE
    deadcode_game
    deadcode_engine
)

# -----------------------------------------------------------------------------
# Assets Installation
# -----------------------------------------------------------------------------
# Copy assets to build directory
add_custom_command(TARGET deadcode_rpg POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_ASSETS_DIR}
    $<TARGET_FILE_DIR:deadcode_rpg>/assets
  COMMENT "Copying assets to build directory"
)

# -----------------------------------------------------------------------------
# Testing
# -----------------------------------------------------------------------------
if(BUILD_TESTS)
  enable_testing()
  find_package(GTest CONFIG REQUIRED)

  add_executable(deadcode_tests
    tests/main.cpp
    tests/core/test_logger.cpp
    tests/core/test_config.cpp
    tests/game/test_entity.cpp
    tests/game/test_combat.cpp
    tests/game/test_inventory.cpp
  )

  target_link_libraries(deadcode_tests
    PRIVATE
      deadcode_game
      deadcode_engine
      GTest::gtest
      GTest::gtest_main
  )

  include(GoogleTest)
  gtest_discover_tests(deadcode_tests)
endif()

# -----------------------------------------------------------------------------
# Documentation
# -----------------------------------------------------------------------------
if(BUILD_DOCS)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(docs ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM
    )
  else()
    message(WARNING "Doxygen not found, cannot generate documentation")
  endif()
endif()

# -----------------------------------------------------------------------------
# Installation
# -----------------------------------------------------------------------------
include(GNUInstallDirs)

install(TARGETS deadcode_rpg deadcode_engine deadcode_game
  EXPORT DeadCodeRPGTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${PROJECT_INCLUDE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${PROJECT_ASSETS_DIR}/
  DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/assets
)

# -----------------------------------------------------------------------------
# Package Configuration
# -----------------------------------------------------------------------------
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/DeadCodeRPGConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(EXPORT DeadCodeRPGTargets
  FILE DeadCodeRPGTargets.cmake
  NAMESPACE DeadCode::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DeadCodeRPG
)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/DeadCodeRPGConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/DeadCodeRPGConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DeadCodeRPG
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/DeadCodeRPGConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/DeadCodeRPGConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DeadCodeRPG
)

# -----------------------------------------------------------------------------
# Print Configuration Summary
# -----------------------------------------------------------------------------
message(STATUS "")
message(STATUS "=============================================================")
message(STATUS "  0xDEADC0DE Text-Based RPG - Configuration Summary")
message(STATUS "=============================================================")
message(STATUS "  Version:          ${PROJECT_VERSION}")
message(STATUS "  Build type:       ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard:     ${CMAKE_CXX_STANDARD}")
message(STATUS "  Compiler:         ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Install prefix:   ${CMAKE_INSTALL_PREFIX}")
message(STATUS "-------------------------------------------------------------")
message(STATUS "  Options:")
message(STATUS "    BUILD_TESTS:        ${BUILD_TESTS}")
message(STATUS "    BUILD_DOCS:         ${BUILD_DOCS}")
message(STATUS "    ENABLE_SANITIZERS:  ${ENABLE_SANITIZERS}")
message(STATUS "    ENABLE_COVERAGE:    ${ENABLE_COVERAGE}")
message(STATUS "    ENABLE_CLANG_TIDY:  ${ENABLE_CLANG_TIDY}")
message(STATUS "    ENABLE_CPPCHECK:    ${ENABLE_CPPCHECK}")
message(STATUS "=============================================================")
message(STATUS "")
