#!/usr/bin/env bash
# 0xDEADC0DE Clean Script
# Cleans build artifacts

set -e

# Colors
BLUE='\033[0;34m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

print_info "Cleaning build artifacts..."

# Remove build directory
if [ -d "build" ]; then
    print_info "Removing build/ directory..."
    rm -rf build
fi

# Remove compile_commands.json
if [ -f "compile_commands.json" ]; then
    print_info "Removing compile_commands.json..."
    rm -f compile_commands.json
fi

# Remove vcpkg_installed directory
if [ -d "vcpkg_installed" ]; then
    print_warning "Removing vcpkg_installed/ directory..."
    rm -rf vcpkg_installed
fi

print_success "Clean complete!"
