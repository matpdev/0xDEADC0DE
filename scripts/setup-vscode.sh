#!/usr/bin/env bash
###############################################################################
# setup-vscode.sh
# Generate VS Code configuration files for 0xDEADC0DE project
#
# Usage: ./scripts/setup-vscode.sh
###############################################################################

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Project directories
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
VSCODE_DIR="${PROJECT_DIR}/.vscode"

echo -e "${BLUE}[INFO] 0xDEADC0DE VS Code Setup${NC}"
echo -e "${BLUE}==============================${NC}"

# Create .vscode directory if it doesn't exist
mkdir -p "${VSCODE_DIR}"

# Detect VCPKG_ROOT
if [ -z "${VCPKG_ROOT}" ]; then
    if [ -d "${HOME}/vcpkg" ]; then
        VCPKG_ROOT="${HOME}/vcpkg"
    elif [ -d "/home/mistergrow/vcpkg" ]; then
        VCPKG_ROOT="/home/mistergrow/vcpkg"
    else
        echo -e "${YELLOW}[WARN] VCPKG_ROOT not set, using default: ~/vcpkg${NC}"
        VCPKG_ROOT="${HOME}/vcpkg"
    fi
fi

echo -e "${GREEN}[INFO] VCPKG_ROOT: ${VCPKG_ROOT}${NC}"

# Detect compiler
CXX_COMPILER=$(which g++ || echo "/usr/bin/g++")
C_COMPILER=$(which gcc || echo "/usr/bin/gcc")
CXX_VERSION=$(${CXX_COMPILER} --version | head -n1 || echo "Unknown")

echo -e "${GREEN}[INFO] C++ Compiler: ${CXX_COMPILER}${NC}"
echo -e "${GREEN}[INFO] Version: ${CXX_VERSION}${NC}"

# Detect number of CPU cores
NPROC=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo "4")
echo -e "${GREEN}[INFO] CPU Cores: ${NPROC}${NC}"

###############################################################################
# Generate c_cpp_properties.json
###############################################################################
echo -e "${BLUE}[INFO] Generating c_cpp_properties.json...${NC}"

cat > "${VSCODE_DIR}/c_cpp_properties.json" << EOF
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "\${workspaceFolder}/include/**",
                "\${workspaceFolder}/src/**",
                "\${workspaceFolder}/build/vcpkg_installed/x64-linux/include/**",
                "\${env:VCPKG_ROOT}/installed/x64-linux/include/**",
                "/usr/include/**",
                "/usr/local/include/**"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "compilerPath": "${CXX_COMPILER}",
            "cStandard": "c11",
            "cppStandard": "c++20",
            "intelliSenseMode": "linux-gcc-x64",
            "compileCommands": "\${workspaceFolder}/compile_commands.json",
            "configurationProvider": "ms-vscode.cmake-tools",
            "browse": {
                "path": [
                    "\${workspaceFolder}/include",
                    "\${workspaceFolder}/src",
                    "\${workspaceFolder}/build/vcpkg_installed/x64-linux/include",
                    "/usr/include",
                    "/usr/local/include"
                ],
                "limitSymbolsToIncludedHeaders": true,
                "databaseFilename": "\${workspaceFolder}/.vscode/browse.vc.db"
            }
        }
    ],
    "version": 4
}
EOF

echo -e "${GREEN}[SUCCESS] c_cpp_properties.json created${NC}"

###############################################################################
# Generate cmake-kits.json
###############################################################################
echo -e "${BLUE}[INFO] Generating cmake-kits.json...${NC}"

cat > "${VSCODE_DIR}/cmake-kits.json" << EOF
[
    {
        "name": "GCC (vcpkg)",
        "compilers": {
            "C": "${C_COMPILER}",
            "CXX": "${CXX_COMPILER}"
        },
        "toolchainFile": "\${env:VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake",
        "preferredGenerator": {
            "name": "Unix Makefiles"
        },
        "environmentVariables": {
            "VCPKG_ROOT": "${VCPKG_ROOT}"
        }
    }
]
EOF

# Add Clang kit if available
if command -v clang++ &> /dev/null; then
    CLANG_CXX=$(which clang++)
    CLANG_C=$(which clang)
    CLANG_VERSION=$(${CLANG_CXX} --version | head -n1 || echo "Unknown")
    echo -e "${GREEN}[INFO] Clang found: ${CLANG_VERSION}${NC}"

    # Append Clang kit to cmake-kits.json
    cat > "${VSCODE_DIR}/cmake-kits.json" << EOF
[
    {
        "name": "GCC (vcpkg)",
        "compilers": {
            "C": "${C_COMPILER}",
            "CXX": "${CXX_COMPILER}"
        },
        "toolchainFile": "\${env:VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake",
        "preferredGenerator": {
            "name": "Unix Makefiles"
        },
        "environmentVariables": {
            "VCPKG_ROOT": "${VCPKG_ROOT}"
        }
    },
    {
        "name": "Clang (vcpkg)",
        "compilers": {
            "C": "${CLANG_C}",
            "CXX": "${CLANG_CXX}"
        },
        "toolchainFile": "\${env:VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake",
        "preferredGenerator": {
            "name": "Unix Makefiles"
        },
        "environmentVariables": {
            "VCPKG_ROOT": "${VCPKG_ROOT}"
        }
    }
]
EOF
fi

echo -e "${GREEN}[SUCCESS] cmake-kits.json created${NC}"

###############################################################################
# Update or create settings.json
###############################################################################
echo -e "${BLUE}[INFO] Updating settings.json...${NC}"

cat > "${VSCODE_DIR}/settings.json" << EOF
{
    "files.associations": {
        "*.hpp": "cpp",
        "*.cpp": "cpp",
        "*.h": "c",
        "*.c": "c"
    },
    "editor.formatOnSave": true,
    "editor.rulers": [100],
    "editor.tabSize": 4,
    "editor.insertSpaces": true,
    "files.trimTrailingWhitespace": true,
    "files.insertFinalNewline": true,
    "files.exclude": {
        "**/.git": true,
        "**/build": false,
        "**/*.a": true,
        "**/*.o": true
    },
    "search.exclude": {
        "**/build": true,
        "**/vcpkg_installed": true,
        "**/.git": true
    },
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
    "C_Cpp.default.cppStandard": "c++20",
    "C_Cpp.default.cStandard": "c11",
    "C_Cpp.default.compilerPath": "${CXX_COMPILER}",
    "C_Cpp.default.intelliSenseMode": "linux-gcc-x64",
    "C_Cpp.errorSquiggles": "enabled",
    "C_Cpp.clang_format_fallbackStyle": "file",
    "cmake.buildDirectory": "\${workspaceFolder}/build",
    "cmake.configureOnOpen": false,
    "cmake.generator": "Unix Makefiles",
    "cmake.buildArgs": ["-j${NPROC}"],
    "cmake.configureArgs": [
        "-DCMAKE_BUILD_TYPE=Release",
        "-DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    ],
    "cmake.installPrefix": "/usr/local",
    "terminal.integrated.env.linux": {
        "VCPKG_ROOT": "${VCPKG_ROOT}"
    },
    "[cpp]": {
        "editor.defaultFormatter": "xaver.clang-format",
        "editor.formatOnSave": true
    },
    "[c]": {
        "editor.defaultFormatter": "xaver.clang-format",
        "editor.formatOnSave": true
    },
    "[json]": {
        "editor.defaultFormatter": "vscode.json-language-features"
    },
    "[cmake]": {
        "editor.defaultFormatter": "cheshirekow.cmake-format"
    }
}
EOF

echo -e "${GREEN}[SUCCESS] settings.json updated${NC}"

###############################################################################
# Update or create tasks.json
###############################################################################
echo -e "${BLUE}[INFO] Updating tasks.json...${NC}"

cat > "${VSCODE_DIR}/tasks.json" << EOF
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build (Release)",
            "type": "shell",
            "command": "\${workspaceFolder}/scripts/build.sh",
            "args": ["release"],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "presentation": {
                "reveal": "always",
                "panel": "shared"
            },
            "problemMatcher": "\$gcc"
        },
        {
            "label": "Build (Debug)",
            "type": "shell",
            "command": "\${workspaceFolder}/scripts/build.sh",
            "args": ["debug"],
            "group": "build",
            "presentation": {
                "reveal": "always",
                "panel": "shared"
            },
            "problemMatcher": "\$gcc"
        },
        {
            "label": "Clean Build",
            "type": "shell",
            "command": "\${workspaceFolder}/scripts/clean.sh",
            "presentation": {
                "reveal": "always",
                "panel": "shared"
            },
            "problemMatcher": []
        },
        {
            "label": "Run Game",
            "type": "shell",
            "command": "\${workspaceFolder}/scripts/run.sh",
            "presentation": {
                "reveal": "always",
                "panel": "dedicated",
                "focus": true
            },
            "problemMatcher": [],
            "dependsOn": ["Build (Release)"]
        },
        {
            "label": "Format Code",
            "type": "shell",
            "command": "\${workspaceFolder}/scripts/format.sh",
            "presentation": {
                "reveal": "always",
                "panel": "shared"
            },
            "problemMatcher": []
        },
        {
            "label": "Lint Code",
            "type": "shell",
            "command": "\${workspaceFolder}/scripts/lint.sh",
            "presentation": {
                "reveal": "always",
                "panel": "shared"
            },
            "problemMatcher": []
        },
        {
            "label": "Run Tests",
            "type": "shell",
            "command": "\${workspaceFolder}/scripts/test.sh",
            "presentation": {
                "reveal": "always",
                "panel": "shared"
            },
            "problemMatcher": "\$gcc",
            "group": {
                "kind": "test",
                "isDefault": true
            }
        },
        {
            "label": "CMake Configure",
            "type": "shell",
            "command": "cmake",
            "args": [
                "-B", "\${workspaceFolder}/build",
                "-S", "\${workspaceFolder}",
                "-DCMAKE_BUILD_TYPE=Release",
                "-DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake",
                "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
            ],
            "presentation": {
                "reveal": "always",
                "panel": "shared"
            },
            "problemMatcher": []
        },
        {
            "label": "CMake Build",
            "type": "shell",
            "command": "cmake",
            "args": [
                "--build", "\${workspaceFolder}/build",
                "--config", "Release",
                "-j", "${NPROC}"
            ],
            "presentation": {
                "reveal": "always",
                "panel": "shared"
            },
            "problemMatcher": "\$gcc",
            "group": "build"
        },
        {
            "label": "Setup VS Code Config",
            "type": "shell",
            "command": "\${workspaceFolder}/scripts/setup-vscode.sh",
            "presentation": {
                "reveal": "always",
                "panel": "shared"
            },
            "problemMatcher": []
        }
    ]
}
EOF

echo -e "${GREEN}[SUCCESS] tasks.json updated${NC}"

###############################################################################
# Generate or update launch.json
###############################################################################
echo -e "${BLUE}[INFO] Updating launch.json...${NC}"

cat > "${VSCODE_DIR}/launch.json" << EOF
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug Game (GDB)",
            "type": "cppdbg",
            "request": "launch",
            "program": "\${workspaceFolder}/build/bin/deadcode_rpg",
            "args": [],
            "stopAtEntry": false,
            "cwd": "\${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                },
                {
                    "description": "Set Disassembly Flavor to Intel",
                    "text": "-gdb-set disassembly-flavor intel",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "Build (Debug)",
            "miDebuggerPath": "/usr/bin/gdb",
            "logging": {
                "engineLogging": false
            }
        },
        {
            "name": "Debug Game (Current)",
            "type": "cppdbg",
            "request": "launch",
            "program": "\${workspaceFolder}/build/bin/deadcode_rpg",
            "args": [],
            "stopAtEntry": false,
            "cwd": "\${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "miDebuggerPath": "/usr/bin/gdb"
        },
        {
            "name": "Release Game (Run)",
            "type": "cppdbg",
            "request": "launch",
            "program": "\${workspaceFolder}/build/bin/deadcode_rpg",
            "args": [],
            "stopAtEntry": false,
            "cwd": "\${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "preLaunchTask": "Build (Release)"
        }
    ]
}
EOF

echo -e "${GREEN}[SUCCESS] launch.json updated${NC}"

###############################################################################
# Generate or update extensions.json
###############################################################################
echo -e "${BLUE}[INFO] Updating extensions.json...${NC}"

cat > "${VSCODE_DIR}/extensions.json" << EOF
{
    "recommendations": [
        "ms-vscode.cpptools",
        "ms-vscode.cpptools-extension-pack",
        "ms-vscode.cmake-tools",
        "twxs.cmake",
        "xaver.clang-format",
        "notskm.clang-tidy",
        "cheshirekow.cmake-format",
        "jeff-hykin.better-cpp-syntax",
        "cschlosser.doxdocgen",
        "zxh404.vscode-proto3",
        "gruntfuggly.todo-tree",
        "shardulm94.trailing-spaces",
        "streetsidesoftware.code-spell-checker",
        "aaron-bond.better-comments",
        "eamodio.gitlens",
        "github.copilot"
    ],
    "unwantedRecommendations": []
}
EOF

echo -e "${GREEN}[SUCCESS] extensions.json updated${NC}"

###############################################################################
# Summary
###############################################################################
echo ""
echo -e "${GREEN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║        VS Code Configuration Generated Successfully!      ║${NC}"
echo -e "${GREEN}╚════════════════════════════════════════════════════════════╝${NC}"
echo ""
echo -e "${BLUE}Generated files:${NC}"
echo -e "  ✓ ${VSCODE_DIR}/c_cpp_properties.json"
echo -e "  ✓ ${VSCODE_DIR}/cmake-kits.json"
echo -e "  ✓ ${VSCODE_DIR}/settings.json"
echo -e "  ✓ ${VSCODE_DIR}/tasks.json"
echo -e "  ✓ ${VSCODE_DIR}/launch.json"
echo -e "  ✓ ${VSCODE_DIR}/extensions.json"
echo ""
echo -e "${BLUE}Configuration:${NC}"
echo -e "  Compiler:     ${CXX_COMPILER}"
echo -e "  C++ Standard: C++20"
echo -e "  C Standard:   C11"
echo -e "  VCPKG_ROOT:   ${VCPKG_ROOT}"
echo -e "  Parallel Jobs: ${NPROC}"
echo ""
echo -e "${YELLOW}Next steps:${NC}"
echo -e "  1. Reload VS Code window (Ctrl+Shift+P -> 'Reload Window')"
echo -e "  2. Install recommended extensions if prompted"
echo -e "  3. Press F5 to debug or Ctrl+Shift+B to build"
echo ""
