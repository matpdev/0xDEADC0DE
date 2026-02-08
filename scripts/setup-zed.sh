#!/usr/bin/env bash
###############################################################################
# setup-zed.sh
# Generate Zed editor configuration files for 0xDEADC0DE project
#
# Usage: ./scripts/setup-zed.sh
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
ZED_DIR="${PROJECT_DIR}/.zed"

echo -e "${BLUE}[INFO] 0xDEADC0DE Zed Editor Setup${NC}"
echo -e "${BLUE}====================================${NC}"

# Create .zed directory if it doesn't exist
mkdir -p "${ZED_DIR}"

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

# Detect compilers
CXX_COMPILER=$(which g++ 2>/dev/null || echo "/usr/bin/g++")
C_COMPILER=$(which gcc 2>/dev/null || echo "/usr/bin/gcc")
CXX_VERSION=$(${CXX_COMPILER} --version 2>/dev/null | head -n1 || echo "Unknown")

echo -e "${GREEN}[INFO] C++ Compiler: ${CXX_COMPILER}${NC}"
echo -e "${GREEN}[INFO] Version: ${CXX_VERSION}${NC}"

# Detect clangd
CLANGD_BIN=$(which clangd 2>/dev/null || echo "")
if [ -n "${CLANGD_BIN}" ]; then
    CLANGD_VERSION=$(${CLANGD_BIN} --version 2>/dev/null | head -n1 || echo "Unknown")
    echo -e "${GREEN}[INFO] clangd: ${CLANGD_BIN}${NC}"
    echo -e "${GREEN}[INFO] Version: ${CLANGD_VERSION}${NC}"
else
    echo -e "${YELLOW}[WARN] clangd not found. C++ language support will be limited.${NC}"
    echo -e "${YELLOW}       Install with: sudo apt install clangd (or sudo pacman -S clang)${NC}"
fi

# Detect clang-format
CLANG_FORMAT_BIN=$(which clang-format 2>/dev/null || echo "")
if [ -n "${CLANG_FORMAT_BIN}" ]; then
    echo -e "${GREEN}[INFO] clang-format: ${CLANG_FORMAT_BIN}${NC}"
else
    echo -e "${YELLOW}[WARN] clang-format not found. Format on save will not work.${NC}"
fi

# Detect number of CPU cores
NPROC=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo "4")
echo -e "${GREEN}[INFO] CPU Cores: ${NPROC}${NC}"

###############################################################################
# Ensure compile_commands.json is available
###############################################################################
if [ -f "${PROJECT_DIR}/build/compile_commands.json" ] && [ ! -f "${PROJECT_DIR}/compile_commands.json" ]; then
    echo -e "${BLUE}[INFO] Symlinking compile_commands.json to project root...${NC}"
    ln -sf "${PROJECT_DIR}/build/compile_commands.json" "${PROJECT_DIR}/compile_commands.json"
    echo -e "${GREEN}[SUCCESS] compile_commands.json symlinked${NC}"
elif [ ! -f "${PROJECT_DIR}/compile_commands.json" ]; then
    echo -e "${YELLOW}[WARN] compile_commands.json not found.${NC}"
    echo -e "${YELLOW}       Run ./scripts/build.sh first to generate it for clangd.${NC}"
fi

###############################################################################
# Generate settings.json
###############################################################################
echo -e "${BLUE}[INFO] Generating settings.json...${NC}"

# Build clangd arguments
CLANGD_ARGS="\"--background-index\", \"--clang-tidy\", \"--header-insertion=iwyu\", \"--completion-style=detailed\", \"--function-arg-placeholders\", \"--fallback-style=file\", \"-j=${NPROC}\""

# Add compile-commands-dir if build directory exists
if [ -d "${PROJECT_DIR}/build" ]; then
    CLANGD_ARGS="\"--compile-commands-dir=${PROJECT_DIR}/build\", ${CLANGD_ARGS}"
fi

# Build clangd LSP block
if [ -n "${CLANGD_BIN}" ]; then
    CLANGD_BLOCK=$(cat << CLANGD
    "clangd": {
      "binary": {
        "path": "${CLANGD_BIN}",
        "arguments": [${CLANGD_ARGS}]
      }
    }
CLANGD
)
else
    CLANGD_BLOCK=$(cat << CLANGD
    "clangd": {
      "binary": {
        "arguments": [${CLANGD_ARGS}]
      }
    }
CLANGD
)
fi

cat > "${ZED_DIR}/settings.json" << EOF
{
  "tab_size": 4,
  "hard_tabs": false,
  "rulers": [100],
  "preferred_line_length": 100,
  "remove_trailing_whitespace_on_save": true,
  "ensure_final_newline_on_save": true,
  "format_on_save": "on",
  "show_whitespaces": "boundary",
  "file_scan_exclusions": [
    "**/.git",
    "**/build",
    "**/vcpkg_installed",
    "**/.cache",
    "**/.clangd",
    "**/*.o",
    "**/*.a",
    "**/*.so",
    "**/*.dylib"
  ],
  "file_types": {
    "C++": ["hpp", "cpp", "h"],
    "C": ["c"],
    "JSON": ["json"],
    "GLSL": ["vert", "frag", "glsl", "geom", "comp"]
  },
  "languages": {
    "C++": {
      "tab_size": 4,
      "hard_tabs": false,
      "format_on_save": "on",
      "formatter": {
        "external": {
          "command": "clang-format",
          "arguments": ["--assume-filename", "{buffer_path}"]
        }
      }
    },
    "C": {
      "tab_size": 4,
      "hard_tabs": false,
      "format_on_save": "on",
      "formatter": {
        "external": {
          "command": "clang-format",
          "arguments": ["--assume-filename", "{buffer_path}"]
        }
      }
    },
    "JSON": {
      "tab_size": 4,
      "format_on_save": "on"
    },
    "GLSL": {
      "tab_size": 4
    }
  },
  "lsp": {
${CLANGD_BLOCK}
  },
  "terminal": {
    "env": {
      "VCPKG_ROOT": "${VCPKG_ROOT}"
    }
  }
}
EOF

echo -e "${GREEN}[SUCCESS] settings.json created${NC}"

###############################################################################
# Generate tasks.json
###############################################################################
echo -e "${BLUE}[INFO] Generating tasks.json...${NC}"

cat > "${ZED_DIR}/tasks.json" << 'EOF'
[
  {
    "label": "Build (Debug)",
    "command": "./scripts/build.sh",
    "args": ["debug"],
    "use_new_terminal": false,
    "allow_concurrent_runs": false,
    "reveal": "always",
    "tags": ["build"]
  },
  {
    "label": "Build (Release)",
    "command": "./scripts/build.sh",
    "args": ["release"],
    "use_new_terminal": false,
    "allow_concurrent_runs": false,
    "reveal": "always",
    "tags": ["build"]
  },
  {
    "label": "Build Clean (Debug)",
    "command": "./scripts/build.sh",
    "args": ["debug", "--clean"],
    "use_new_terminal": false,
    "allow_concurrent_runs": false,
    "reveal": "always",
    "tags": ["build"]
  },
  {
    "label": "Build Clean (Release)",
    "command": "./scripts/build.sh",
    "args": ["release", "--clean"],
    "use_new_terminal": false,
    "allow_concurrent_runs": false,
    "reveal": "always",
    "tags": ["build"]
  },
  {
    "label": "Build with Tests (Debug)",
    "command": "./scripts/build.sh",
    "args": ["debug", "--tests"],
    "use_new_terminal": false,
    "allow_concurrent_runs": false,
    "reveal": "always",
    "tags": ["build"]
  },
  {
    "label": "Build with Sanitizers (Debug)",
    "command": "./scripts/build.sh",
    "args": ["debug", "--sanitizers"],
    "use_new_terminal": false,
    "allow_concurrent_runs": false,
    "reveal": "always",
    "tags": ["build"]
  },
  {
    "label": "Run Game",
    "command": "./scripts/run.sh",
    "use_new_terminal": true,
    "allow_concurrent_runs": false,
    "reveal": "always",
    "tags": ["run"]
  },
  {
    "label": "Format Code",
    "command": "./scripts/format.sh",
    "use_new_terminal": false,
    "allow_concurrent_runs": false,
    "reveal": "always"
  },
  {
    "label": "Check Formatting",
    "command": "./scripts/format.sh",
    "args": ["--check"],
    "use_new_terminal": false,
    "allow_concurrent_runs": false,
    "reveal": "always"
  },
  {
    "label": "Lint (clang-tidy)",
    "command": "./scripts/lint.sh",
    "use_new_terminal": false,
    "allow_concurrent_runs": false,
    "reveal": "always"
  },
  {
    "label": "Run Tests",
    "command": "./scripts/test.sh",
    "use_new_terminal": false,
    "allow_concurrent_runs": false,
    "reveal": "always",
    "tags": ["test"]
  },
  {
    "label": "CMake Configure (Debug)",
    "command": "cmake",
    "args": ["-S", ".", "-B", "build", "-DCMAKE_BUILD_TYPE=Debug", "-DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"],
    "use_new_terminal": false,
    "allow_concurrent_runs": false,
    "reveal": "always",
    "tags": ["build"]
  },
  {
    "label": "CMake Configure (Release)",
    "command": "cmake",
    "args": ["-S", ".", "-B", "build", "-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"],
    "use_new_terminal": false,
    "allow_concurrent_runs": false,
    "reveal": "always",
    "tags": ["build"]
  },
  {
    "label": "CMake Build",
    "command": "cmake",
    "args": ["--build", "build", "-j"],
    "use_new_terminal": false,
    "allow_concurrent_runs": false,
    "reveal": "always",
    "tags": ["build"]
  },
  {
    "label": "Setup Zed Config",
    "command": "./scripts/setup-zed.sh",
    "use_new_terminal": false,
    "allow_concurrent_runs": false,
    "reveal": "always"
  }
]
EOF

echo -e "${GREEN}[SUCCESS] tasks.json created${NC}"

###############################################################################
# Summary
###############################################################################
echo ""
echo -e "${GREEN}======================================================${NC}"
echo -e "${GREEN}  Zed Configuration Generated Successfully!${NC}"
echo -e "${GREEN}======================================================${NC}"
echo ""
echo -e "${BLUE}Generated files:${NC}"
echo -e "  ${GREEN}+${NC} ${ZED_DIR}/settings.json"
echo -e "  ${GREEN}+${NC} ${ZED_DIR}/tasks.json"
echo ""
echo -e "${BLUE}Configuration:${NC}"
echo -e "  Compiler:       ${CXX_COMPILER}"
echo -e "  C++ Standard:   C++20"
echo -e "  C Standard:     C17"
echo -e "  VCPKG_ROOT:     ${VCPKG_ROOT}"
echo -e "  clangd:         ${CLANGD_BIN:-not found}"
echo -e "  clang-format:   ${CLANG_FORMAT_BIN:-not found}"
echo -e "  Parallel Jobs:  ${NPROC}"
echo ""
echo -e "${BLUE}Features:${NC}"
echo -e "  ${GREEN}+${NC} clangd LSP (intellisense, diagnostics, clang-tidy)"
echo -e "  ${GREEN}+${NC} clang-format on save (uses project .clang-format)"
echo -e "  ${GREEN}+${NC} GLSL shader file support"
echo -e "  ${GREEN}+${NC} Build/Run/Format/Lint tasks"
echo -e "  ${GREEN}+${NC} VCPKG_ROOT in terminal environment"
echo ""
echo -e "${YELLOW}Next steps:${NC}"
echo -e "  1. Open project in Zed: zed ${PROJECT_DIR}"
echo -e "  2. Run tasks via command palette: Ctrl+Shift+P -> 'task: spawn'"
echo -e "  3. Build with Ctrl+Shift+B (if keybinding is set)"
echo ""
if [ -z "${CLANGD_BIN}" ]; then
    echo -e "${YELLOW}Recommended:${NC}"
    echo -e "  Install clangd for full C++ support:"
    echo -e "    Arch:   sudo pacman -S clang"
    echo -e "    Ubuntu: sudo apt install clangd"
    echo -e "    Fedora: sudo dnf install clang-tools-extra"
    echo ""
fi
if [ ! -f "${PROJECT_DIR}/compile_commands.json" ] && [ ! -f "${PROJECT_DIR}/build/compile_commands.json" ]; then
    echo -e "${YELLOW}Recommended:${NC}"
    echo -e "  Run ./scripts/build.sh to generate compile_commands.json for clangd"
    echo ""
fi
