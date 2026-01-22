#!/usr/bin/env bash
# 0xDEADC0DE Build Script
# Usage: ./build.sh [debug|release|relwithdebinfo] [options]

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Default values
BUILD_TYPE="Release"
BUILD_DIR="build"
CLEAN_BUILD=false
ENABLE_TESTS=false
ENABLE_DOCS=false
ENABLE_SANITIZERS=false
ENABLE_CLANG_TIDY=false
ENABLE_CPPCHECK=false
PARALLEL_JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Function to print colored messages
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to display usage
usage() {
    cat << EOF
Usage: $0 [BUILD_TYPE] [OPTIONS]

BUILD_TYPE:
    debug           Build with debug symbols and no optimization
    release         Build with optimizations (default)
    relwithdebinfo  Build with optimizations and debug symbols

OPTIONS:
    --clean         Clean build directory before building
    --tests         Enable unit tests
    --docs          Enable documentation generation
    --sanitizers    Enable address and undefined behavior sanitizers
    --clang-tidy    Enable clang-tidy static analysis
    --cppcheck      Enable cppcheck static analysis
    --jobs N        Number of parallel build jobs (default: $PARALLEL_JOBS)
    --help          Display this help message

EXAMPLES:
    $0                              # Release build
    $0 debug                        # Debug build
    $0 release --tests              # Release build with tests
    $0 debug --sanitizers --tests   # Debug build with sanitizers and tests
    $0 --clean --tests --docs       # Clean release build with tests and docs

ENVIRONMENT VARIABLES:
    VCPKG_ROOT      Path to vcpkg installation (required)
    CXX             C++ compiler to use (default: auto-detected)
    CC              C compiler to use (default: auto-detected)

EOF
    exit 0
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        debug|DEBUG)
            BUILD_TYPE="Debug"
            shift
            ;;
        release|RELEASE)
            BUILD_TYPE="Release"
            shift
            ;;
        relwithdebinfo|RELWITHDEBINFO)
            BUILD_TYPE="RelWithDebInfo"
            shift
            ;;
        --clean)
            CLEAN_BUILD=true
            shift
            ;;
        --tests)
            ENABLE_TESTS=true
            shift
            ;;
        --docs)
            ENABLE_DOCS=true
            shift
            ;;
        --sanitizers)
            ENABLE_SANITIZERS=true
            shift
            ;;
        --clang-tidy)
            ENABLE_CLANG_TIDY=true
            shift
            ;;
        --cppcheck)
            ENABLE_CPPCHECK=true
            shift
            ;;
        --jobs)
            PARALLEL_JOBS="$2"
            shift 2
            ;;
        --help|-h)
            usage
            ;;
        *)
            print_error "Unknown option: $1"
            usage
            ;;
    esac
done

# Check for vcpkg
if [ -z "$VCPKG_ROOT" ]; then
    print_error "VCPKG_ROOT environment variable is not set"
    print_info "Please set VCPKG_ROOT to your vcpkg installation directory"
    print_info "Example: export VCPKG_ROOT=/path/to/vcpkg"
    exit 1
fi

if [ ! -f "$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" ]; then
    print_error "vcpkg toolchain file not found at: $VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
    exit 1
fi

# Project root directory
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

print_info "0xDEADC0DE Build Script"
print_info "======================="
print_info "Build type: $BUILD_TYPE"
print_info "Build directory: $BUILD_DIR"
print_info "Parallel jobs: $PARALLEL_JOBS"
print_info "Tests: $([ "$ENABLE_TESTS" = true ] && echo "Enabled" || echo "Disabled")"
print_info "Docs: $([ "$ENABLE_DOCS" = true ] && echo "Enabled" || echo "Disabled")"
print_info "Sanitizers: $([ "$ENABLE_SANITIZERS" = true ] && echo "Enabled" || echo "Disabled")"

# Clean build directory if requested
if [ "$CLEAN_BUILD" = true ]; then
    print_info "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory
mkdir -p "$BUILD_DIR"

# Configure CMake options
CMAKE_OPTIONS=(
    -S .
    -B "$BUILD_DIR"
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
)

if [ "$ENABLE_TESTS" = true ]; then
    CMAKE_OPTIONS+=(-DBUILD_TESTS=ON)
fi

if [ "$ENABLE_DOCS" = true ]; then
    CMAKE_OPTIONS+=(-DBUILD_DOCS=ON)
fi

if [ "$ENABLE_SANITIZERS" = true ]; then
    CMAKE_OPTIONS+=(-DENABLE_SANITIZERS=ON)
fi

if [ "$ENABLE_CLANG_TIDY" = true ]; then
    CMAKE_OPTIONS+=(-DENABLE_CLANG_TIDY=ON)
fi

if [ "$ENABLE_CPPCHECK" = true ]; then
    CMAKE_OPTIONS+=(-DENABLE_CPPCHECK=ON)
fi

# Configure step
print_info "Configuring CMake..."
if ! cmake "${CMAKE_OPTIONS[@]}"; then
    print_error "CMake configuration failed"
    exit 1
fi

print_success "CMake configuration completed"

# Build step
print_info "Building project with $PARALLEL_JOBS parallel jobs..."
if ! cmake --build "$BUILD_DIR" --config "$BUILD_TYPE" -j "$PARALLEL_JOBS"; then
    print_error "Build failed"
    exit 1
fi

print_success "Build completed successfully"

# Copy compile_commands.json to project root for IDE integration
if [ -f "$BUILD_DIR/compile_commands.json" ]; then
    cp "$BUILD_DIR/compile_commands.json" "$PROJECT_ROOT/"
    print_info "compile_commands.json copied to project root"
fi

# Print summary
echo ""
print_success "╔════════════════════════════════════════════════════════════╗"
print_success "║              Build Completed Successfully!                ║"
print_success "╚════════════════════════════════════════════════════════════╝"
echo ""
print_info "Executable: $BUILD_DIR/bin/deadcode_rpg"
print_info "Libraries: $BUILD_DIR/lib/"
if [ "$ENABLE_TESTS" = true ]; then
    print_info "Tests: $BUILD_DIR/bin/deadcode_tests"
fi
echo ""
print_info "To run the game: ./build/bin/deadcode_rpg"
print_info "Or use: ./scripts/run.sh"
if [ "$ENABLE_TESTS" = true ]; then
    echo ""
    print_info "To run tests: ./scripts/test.sh"
fi
