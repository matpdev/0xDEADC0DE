#!/usr/bin/env bash
# 0xDEADC0DE Test Script
# Runs unit tests

set -e

# Colors
BLUE='\033[0;34m'
GREEN='\033[0;32m'
RED='\033[0;31m'
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

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

# Check if tests are built
if [ ! -f "build/bin/deadcode_tests" ]; then
    print_info "Tests not built. Building with tests enabled..."
    ./scripts/build.sh --tests
fi

# Run tests
print_info "Running tests..."
cd build
ctest --output-on-failure "$@"

print_success "All tests passed!"
