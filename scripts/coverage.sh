#!/bin/bash
# DIPAL Code Coverage Script
# Usage: ./scripts/coverage.sh [options]

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Default values
BUILD_DIR="build-coverage"
OUTPUT_DIR="coverage"
OPEN_REPORT=false

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

usage() {
    echo "DIPAL Code Coverage Script"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -o, --open      Open HTML report after generation"
    echo "  -c, --clean     Clean and rebuild coverage build"
    echo "  -h, --help      Show this help message"
    echo ""
    echo "Requirements:"
    echo "  - GCC or Clang compiler"
    echo "  - lcov and genhtml (for HTML reports)"
    echo ""
    echo "Install on Ubuntu: sudo apt install lcov"
    echo "Install on macOS:  brew install lcov"
}

# Parse arguments
CLEAN_BUILD=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -o|--open)
            OPEN_REPORT=true
            shift
            ;;
        -c|--clean)
            CLEAN_BUILD=true
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
echo -e "${BLUE}  DIPAL Code Coverage${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""

cd "$PROJECT_ROOT"

# Check for lcov
if ! command -v lcov &> /dev/null; then
    echo -e "${RED}Error: lcov not found${NC}"
    echo "Please install lcov:"
    echo "  Ubuntu/Debian: sudo apt install lcov"
    echo "  macOS: brew install lcov"
    exit 1
fi

# Clean if requested
if [ "$CLEAN_BUILD" = true ]; then
    echo -e "${YELLOW}Cleaning coverage build...${NC}"
    rm -rf "$BUILD_DIR" "$OUTPUT_DIR"
fi

# Configure with coverage flags
echo -e "${YELLOW}Configuring coverage build...${NC}"
mkdir -p "$BUILD_DIR"

cmake -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_FLAGS="--coverage -fprofile-arcs -ftest-coverage" \
    -DCMAKE_C_FLAGS="--coverage -fprofile-arcs -ftest-coverage" \
    -DCMAKE_EXE_LINKER_FLAGS="--coverage" \
    -DBUILD_TESTS=ON \
    -DBUILD_EXAMPLES=OFF

# Build
echo -e "${YELLOW}Building...${NC}"
cmake --build "$BUILD_DIR" --parallel

# Reset coverage counters
echo -e "${YELLOW}Resetting coverage counters...${NC}"
lcov --zerocounters --directory "$BUILD_DIR"

# Run tests
echo -e "${YELLOW}Running tests...${NC}"
ctest --test-dir "$BUILD_DIR" --output-on-failure || true

# Capture coverage data
echo -e "${YELLOW}Capturing coverage data...${NC}"
mkdir -p "$OUTPUT_DIR"

lcov --capture \
    --directory "$BUILD_DIR" \
    --output-file "$OUTPUT_DIR/coverage.info" \
    --ignore-errors mismatch

# Remove external/system files from coverage
lcov --remove "$OUTPUT_DIR/coverage.info" \
    '/usr/*' \
    '*/build*/_deps/*' \
    '*/tests/*' \
    '*/examples/*' \
    --output-file "$OUTPUT_DIR/coverage.info" \
    --ignore-errors unused

# Generate HTML report
echo -e "${YELLOW}Generating HTML report...${NC}"
genhtml "$OUTPUT_DIR/coverage.info" \
    --output-directory "$OUTPUT_DIR/html" \
    --title "DIPAL Code Coverage" \
    --legend \
    --show-details

# Print summary
echo ""
echo -e "${GREEN}Coverage report generated!${NC}"
echo -e "  HTML report: ${BLUE}$OUTPUT_DIR/html/index.html${NC}"
echo ""

# Show summary
lcov --summary "$OUTPUT_DIR/coverage.info"

# Open report if requested
if [ "$OPEN_REPORT" = true ]; then
    if command -v xdg-open &> /dev/null; then
        xdg-open "$OUTPUT_DIR/html/index.html"
    elif command -v open &> /dev/null; then
        open "$OUTPUT_DIR/html/index.html"
    else
        echo -e "${YELLOW}Could not open browser automatically.${NC}"
    fi
fi

echo ""
echo -e "${BLUE}=====================================${NC}"
