#!/bin/bash
# DIPAL Test Runner Script
# Usage: ./scripts/test.sh [options]

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Default values
BUILD_DIR="build"
PARALLEL_JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
VERBOSE=false
TEST_FILTER=""
BUILD_FIRST=false

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

usage() {
    echo "DIPAL Test Runner"
    echo ""
    echo "Usage: $0 [options] [test-filter]"
    echo ""
    echo "Options:"
    echo "  -d, --debug         Use debug build directory"
    echo "  -b, --build         Build before testing"
    echo "  -j, --jobs N        Number of parallel test jobs (default: $PARALLEL_JOBS)"
    echo "  -v, --verbose       Verbose output (show all test output)"
    echo "  -l, --list          List available tests"
    echo "  -h, --help          Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                  # Run all tests"
    echo "  $0 -v               # Run all tests with verbose output"
    echo "  $0 -b               # Build and run tests"
    echo "  $0 \"Image*\"         # Run tests matching pattern"
    echo "  $0 -l               # List available tests"
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_DIR="build-debug"
            shift
            ;;
        -b|--build)
            BUILD_FIRST=true
            shift
            ;;
        -j|--jobs)
            PARALLEL_JOBS="$2"
            shift 2
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -l|--list)
            cd "$PROJECT_ROOT"
            if [ -d "$BUILD_DIR" ]; then
                ctest --test-dir "$BUILD_DIR" -N
            else
                echo -e "${RED}Build directory not found. Run build first.${NC}"
                exit 1
            fi
            exit 0
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        -*)
            echo -e "${RED}Unknown option: $1${NC}"
            usage
            exit 1
            ;;
        *)
            TEST_FILTER="$1"
            shift
            ;;
    esac
done

echo -e "${BLUE}=====================================${NC}"
echo -e "${BLUE}  DIPAL Test Runner${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""

cd "$PROJECT_ROOT"

# Build first if requested
if [ "$BUILD_FIRST" = true ]; then
    echo -e "${YELLOW}Building...${NC}"
    "$SCRIPT_DIR/build.sh" --jobs "$PARALLEL_JOBS"
    echo ""
fi

# Check build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo -e "${RED}Build directory '$BUILD_DIR' not found.${NC}"
    echo -e "${YELLOW}Run './scripts/build.sh' first or use -b flag.${NC}"
    exit 1
fi

# Build ctest arguments
CTEST_ARGS=(
    --test-dir "$BUILD_DIR"
    --parallel "$PARALLEL_JOBS"
    --output-on-failure
)

if [ "$VERBOSE" = true ]; then
    CTEST_ARGS+=(--verbose)
fi

if [ -n "$TEST_FILTER" ]; then
    CTEST_ARGS+=(-R "$TEST_FILTER")
    echo -e "${YELLOW}Running tests matching: ${GREEN}$TEST_FILTER${NC}"
else
    echo -e "${YELLOW}Running all tests...${NC}"
fi

echo ""

# Run tests
if ctest "${CTEST_ARGS[@]}"; then
    echo ""
    echo -e "${GREEN}All tests passed!${NC}"
    echo -e "${BLUE}=====================================${NC}"
else
    echo ""
    echo -e "${RED}Some tests failed!${NC}"
    echo -e "${BLUE}=====================================${NC}"
    exit 1
fi
