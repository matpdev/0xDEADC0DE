#!/usr/bin/env bash
# 0xDEADC0DE Format Script
# Formats all C++ source files using clang-format

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

# Check if clang-format is installed
if ! command -v clang-format &> /dev/null; then
    print_error "clang-format is not installed"
    print_info "Install it using:"
    print_info "  Ubuntu/Debian: sudo apt install clang-format"
    print_info "  macOS: brew install clang-format"
    exit 1
fi

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

# Check for .clang-format file
if [ ! -f ".clang-format" ]; then
    print_error ".clang-format file not found in project root"
    exit 1
fi

# Parse arguments
CHECK_ONLY=false
if [ "$1" = "--check" ] || [ "$1" = "-c" ]; then
    CHECK_ONLY=true
fi

# Find all C++ source files
print_info "Finding C++ source files..."
FILES=$(find include src tests -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.c" \) 2>/dev/null || true)

if [ -z "$FILES" ]; then
    print_warning "No C++ source files found"
    exit 0
fi

FILE_COUNT=$(echo "$FILES" | wc -l | tr -d ' ')
print_info "Found $FILE_COUNT files to process"

if [ "$CHECK_ONLY" = true ]; then
    print_info "Checking formatting (no changes will be made)..."

    NEEDS_FORMATTING=false
    while IFS= read -r file; do
        if ! clang-format --dry-run --Werror "$file" 2>/dev/null; then
            print_warning "Needs formatting: $file"
            NEEDS_FORMATTING=true
        fi
    done <<< "$FILES"

    if [ "$NEEDS_FORMATTING" = true ]; then
        print_error "Some files need formatting"
        print_info "Run ./scripts/format.sh to format them"
        exit 1
    else
        print_success "All files are properly formatted"
    fi
else
    print_info "Formatting files..."

    FORMATTED=0
    while IFS= read -r file; do
        if clang-format -i "$file"; then
            FORMATTED=$((FORMATTED + 1))
            echo "  Formatted: $file"
        fi
    done <<< "$FILES"

    print_success "Formatted $FORMATTED files"
fi
