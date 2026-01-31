#!/bin/bash
# DIPAL Install Script
# Usage: ./scripts/install.sh [options]

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Default values
BUILD_DIR="build"
PREFIX=""
BUILD_FIRST=false

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

usage() {
    echo "DIPAL Install Script"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -p, --prefix PATH   Installation prefix (default: /usr/local)"
    echo "  -b, --build         Build before installing"
    echo "  -d, --debug         Use debug build"
    echo "  -h, --help          Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 -p ~/local       # Install to ~/local"
    echo "  $0 -b -p ~/local    # Build and install"
    echo "  sudo $0             # Install to /usr/local (requires sudo)"
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -p|--prefix)
            PREFIX="$2"
            shift 2
            ;;
        -b|--build)
            BUILD_FIRST=true
            shift
            ;;
        -d|--debug)
            BUILD_DIR="build-debug"
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            usage
            exit 1
            ;;
    esac
done

echo -e "${BLUE}=====================================${NC}"
echo -e "${BLUE}  DIPAL Install${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""

cd "$PROJECT_ROOT"

# Build first if requested
if [ "$BUILD_FIRST" = true ]; then
    echo -e "${YELLOW}Building...${NC}"
    if [ -n "$PREFIX" ]; then
        "$SCRIPT_DIR/build.sh" --prefix "$PREFIX"
    else
        "$SCRIPT_DIR/build.sh"
    fi
    echo ""
fi

# Check build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo -e "${RED}Build directory '$BUILD_DIR' not found.${NC}"
    echo -e "${YELLOW}Run './scripts/build.sh' first or use -b flag.${NC}"
    exit 1
fi

# Install
echo -e "${YELLOW}Installing...${NC}"

INSTALL_ARGS=(--install "$BUILD_DIR")

if [ -n "$PREFIX" ]; then
    INSTALL_ARGS+=(--prefix "$PREFIX")
    echo -e "  Prefix: ${GREEN}$PREFIX${NC}"
fi

cmake "${INSTALL_ARGS[@]}"

echo ""
echo -e "${GREEN}Installation complete!${NC}"

if [ -n "$PREFIX" ]; then
    echo ""
    echo -e "${YELLOW}To use DIPAL, add to your CMakeLists.txt:${NC}"
    echo ""
    echo "  list(APPEND CMAKE_PREFIX_PATH \"$PREFIX\")"
    echo "  find_package(dipal REQUIRED)"
    echo "  target_link_libraries(your_target PRIVATE DIPAL::dipal)"
fi

echo -e "${BLUE}=====================================${NC}"
