#!/bin/bash
# DIPAL Configure Script
# Usage: ./scripts/configure.sh [options]
#
# This script only configures CMake without building.
# Useful for generating compile_commands.json for IDE integration.

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Default values
BUILD_TYPE="Release"
BUILD_DIR="build"
GENERATOR="Ninja"
BUILD_TESTS="ON"
BUILD_EXAMPLES="ON"

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

usage() {
    echo "DIPAL Configure Script"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -d, --debug         Configure for Debug build"
    echo "  -r, --release       Configure for Release build (default)"
    echo "  -g, --generator G   CMake generator (default: Ninja)"
    echo "  --no-tests          Disable tests"
    echo "  --no-examples       Disable examples"
    echo "  -h, --help          Show this help message"
    echo ""
    echo "This script configures CMake and generates compile_commands.json"
    echo "for IDE integration (VSCode, CLion, Neovim, etc.)"
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="Debug"
            BUILD_DIR="build-debug"
            shift
            ;;
        -r|--release)
            BUILD_TYPE="Release"
            BUILD_DIR="build"
            shift
            ;;
        -g|--generator)
            GENERATOR="$2"
            shift 2
            ;;
        --no-tests)
            BUILD_TESTS="OFF"
            shift
            ;;
        --no-examples)
            BUILD_EXAMPLES="OFF"
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
echo -e "${BLUE}  DIPAL Configure${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""
echo -e "${YELLOW}Configuration:${NC}"
echo -e "  Build Type:  ${GREEN}$BUILD_TYPE${NC}"
echo -e "  Build Dir:   ${GREEN}$BUILD_DIR${NC}"
echo -e "  Generator:   ${GREEN}$GENERATOR${NC}"
echo ""

cd "$PROJECT_ROOT"

# Create build directory
mkdir -p "$BUILD_DIR"

# Configure
echo -e "${YELLOW}Configuring CMake...${NC}"
cmake -B "$BUILD_DIR" \
    -G "$GENERATOR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_CXX_STANDARD=23 \
    -DBUILD_TESTS="$BUILD_TESTS" \
    -DBUILD_EXAMPLES="$BUILD_EXAMPLES" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Link compile_commands.json to project root
if [ -f "$BUILD_DIR/compile_commands.json" ]; then
    ln -sf "$BUILD_DIR/compile_commands.json" compile_commands.json
    echo ""
    echo -e "${GREEN}Created symlink: compile_commands.json -> $BUILD_DIR/compile_commands.json${NC}"
fi

echo ""
echo -e "${GREEN}Configuration complete!${NC}"
echo -e "${YELLOW}Now run './scripts/build.sh' to build the project.${NC}"
echo -e "${BLUE}=====================================${NC}"
