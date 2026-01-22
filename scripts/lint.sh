#!/usr/bin/env bash
# 0xDEADC0DE Lint Script
# Runs clang-tidy on C++ source files

set -e

# Colors
BLUE='\033[0;34m'
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Check if clang-tidy is installed
if ! command -v clang-tidy &> /dev/null; then
    print_error "clang-tidy is not installed"
    print_info "Install it using:"
    print_info "  Ubuntu/Debian: sudo apt install clang-tidy"
    print_info "  macOS: brew install llvm"
    exit 1
fi

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

# Check for .clang-tidy file
if [ ! -f ".clang-tidy" ]; then
    print_error ".clang-tidy file not found in project root"
    exit 1
fi

# Check for compile_commands.json
if [ ! -f "compile_commands.json" ] && [ ! -f "build/compile_commands.json" ]; then
    print_warning "compile_commands.json not found"
    print_info "Building project to generate compile_commands.json..."
    ./scripts/build.sh
fi

# Use compile_commands.json from build directory if not in root
COMPILE_COMMANDS=""
if [ -f "compile_commands.json" ]; then
    COMPILE_COMMANDS="-p ."
elif [ -f "build/compile_commands.json" ]; then
    COMPILE_COMMANDS="-p build"
fi

# Find all C++ source files
print_info "Finding C++ source files..."
FILES=$(find src include -type f \( -name "*.cpp" -o -name "*.hpp" \) 2>/dev/null || true)

if [ -z "$FILES" ]; then
    print_warning "No C++ source files found"
    exit 0
fi

FILE_COUNT=$(echo "$FILES" | wc -l | tr -d ' ')
print_info "Found $FILE_COUNT files to check"
print_info "Running clang-tidy..."

ISSUES_FOUND=false
while IFS= read -r file; do
    echo "  Checking: $file"
    if ! clang-tidy "$file" $COMPILE_COMMANDS 2>&1 | tee /tmp/clang-tidy-output.txt; then
        if grep -q "warning:" /tmp/clang-tidy-output.txt || grep -q "error:" /tmp/clang-tidy-output.txt; then
            ISSUES_FOUND=true
        fi
    fi
done <<< "$FILES"

if [ "$ISSUES_FOUND" = true ]; then
    print_warning "Clang-tidy found some issues"
    exit 1
else
    print_success "No issues found"
fi
