#!/usr/bin/env bash
# 0xDEADC0DE Run Script
# Usage: ./run.sh [options]

set -e

# Colors
BLUE='\033[0;34m'
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Project root
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

# Check if executable exists
EXECUTABLE="build/bin/deadcode_rpg"

if [ ! -f "$EXECUTABLE" ]; then
    print_error "Executable not found: $EXECUTABLE"
    print_info "Please build the project first using: ./scripts/build.sh"
    exit 1
fi

# Run the game
print_info "Starting 0xDEADC0DE..."
"$EXECUTABLE" "$@"
