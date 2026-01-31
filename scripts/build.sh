#!/bin/bash
# DIPAL Build Script
# Usage: ./scripts/build.sh [options]

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
PARALLEL_JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
BUILD_TESTS="ON"
BUILD_EXAMPLES="ON"
CLEAN_BUILD=false
VERBOSE=false
INSTALL=false
INSTALL_PREFIX=""

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

usage() {
    echo "DIPAL Build Script"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -d, --debug         Build in Debug mode (default: Release)"
    echo "  -r, --release       Build in Release mode (default)"
    echo "  -c, --clean         Clean build directory before building"
    echo "  -j, --jobs N        Number of parallel jobs (default: $PARALLEL_JOBS)"
    echo "  -g, --generator G   CMake generator (default: Ninja)"
    echo "  --no-tests          Disable building tests"
    echo "  --no-examples       Disable building examples"
    echo "  --install           Install after building"
    echo "  --prefix PATH       Installation prefix (default: /usr/local)"
    echo "  -v, --verbose       Verbose output"
    echo "  -h, --help          Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                  # Release build with Ninja"
    echo "  $0 -d               # Debug build"
    echo "  $0 -c -d            # Clean debug build"
    echo "  $0 --install --prefix ~/local"
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
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -j|--jobs)
            PARALLEL_JOBS="$2"
            shift 2
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
        --install)
            INSTALL=true
            shift
            ;;
        --prefix)
            INSTALL_PREFIX="$2"
            shift 2
            ;;
        -v|--verbose)
            VERBOSE=true
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
echo -e "${BLUE}  DIPAL Build System${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""
echo -e "${YELLOW}Configuration:${NC}"
echo -e "  Build Type:    ${GREEN}$BUILD_TYPE${NC}"
echo -e "  Build Dir:     ${GREEN}$BUILD_DIR${NC}"
echo -e "  Generator:     ${GREEN}$GENERATOR${NC}"
echo -e "  Jobs:          ${GREEN}$PARALLEL_JOBS${NC}"
echo -e "  Tests:         ${GREEN}$BUILD_TESTS${NC}"
echo -e "  Examples:      ${GREEN}$BUILD_EXAMPLES${NC}"
echo ""

cd "$PROJECT_ROOT"

# Clean if requested
if [ "$CLEAN_BUILD" = true ]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    rm -rf "$BUILD_DIR"
fi

# Create build directory
mkdir -p "$BUILD_DIR"

# Configure
echo -e "${YELLOW}Configuring...${NC}"
CMAKE_ARGS=(
    -B "$BUILD_DIR"
    -G "$GENERATOR"
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
    -DCMAKE_CXX_STANDARD=23
    -DBUILD_TESTS="$BUILD_TESTS"
    -DBUILD_EXAMPLES="$BUILD_EXAMPLES"
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
)

if [ -n "$INSTALL_PREFIX" ]; then
    CMAKE_ARGS+=(-DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX")
fi

if [ "$VERBOSE" = true ]; then
    cmake "${CMAKE_ARGS[@]}"
else
    cmake "${CMAKE_ARGS[@]}" > /dev/null
fi

# Link compile_commands.json to project root
if [ -f "$BUILD_DIR/compile_commands.json" ]; then
    ln -sf "$BUILD_DIR/compile_commands.json" compile_commands.json
fi

# Build
echo -e "${YELLOW}Building...${NC}"
if [ "$VERBOSE" = true ]; then
    cmake --build "$BUILD_DIR" --parallel "$PARALLEL_JOBS"
else
    cmake --build "$BUILD_DIR" --parallel "$PARALLEL_JOBS" 2>&1 | tail -20
fi

# Install if requested
if [ "$INSTALL" = true ]; then
    echo -e "${YELLOW}Installing...${NC}"
    if [ -n "$INSTALL_PREFIX" ]; then
        cmake --install "$BUILD_DIR" --prefix "$INSTALL_PREFIX"
    else
        cmake --install "$BUILD_DIR"
    fi
fi

echo ""
echo -e "${GREEN}Build completed successfully!${NC}"
echo -e "${BLUE}=====================================${NC}"
