#!/bin/bash
# DIPAL Lint Script (clang-tidy)
# Usage: ./scripts/lint.sh [options]

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

BUILD_DIR="build"
FIX=false
VERBOSE=false
FILE_PATTERN=""

usage() {
    echo "DIPAL Lint Script (clang-tidy)"
    echo ""
    echo "Usage: $0 [options] [file-pattern]"
    echo ""
    echo "Options:"
    echo "  -f, --fix       Apply suggested fixes"
    echo "  -d, --debug     Use debug build directory"
    echo "  -v, --verbose   Verbose output"
    echo "  -h, --help      Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                    # Lint all source files"
    echo "  $0 src/Image/*.cpp    # Lint specific files"
    echo "  $0 -f                 # Lint and apply fixes"
    echo ""
    echo "Note: Requires compile_commands.json (run build first)"
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -f|--fix)
            FIX=true
            shift
            ;;
        -d|--debug)
            BUILD_DIR="build-debug"
            shift
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
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
            FILE_PATTERN="$1"
            shift
            ;;
    esac
done

echo -e "${BLUE}=====================================${NC}"
echo -e "${BLUE}  DIPAL Linter (clang-tidy)${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""

cd "$PROJECT_ROOT"

# Check if clang-tidy is available
if ! command -v clang-tidy &> /dev/null; then
    echo -e "${RED}Error: clang-tidy not found${NC}"
    echo "Please install clang-tidy:"
    echo "  Ubuntu/Debian: sudo apt install clang-tidy"
    echo "  macOS: brew install llvm"
    exit 1
fi

# Check for compile_commands.json
COMPILE_DB="$BUILD_DIR/compile_commands.json"
if [ ! -f "$COMPILE_DB" ]; then
    if [ -f "compile_commands.json" ]; then
        COMPILE_DB="compile_commands.json"
    else
        echo -e "${RED}Error: compile_commands.json not found${NC}"
        echo -e "${YELLOW}Run './scripts/build.sh' first to generate it.${NC}"
        exit 1
    fi
fi

echo -e "${YELLOW}Using: $COMPILE_DB${NC}"
echo ""

# Build clang-tidy arguments
TIDY_ARGS=(-p "$COMPILE_DB")

if [ "$FIX" = true ]; then
    TIDY_ARGS+=(-fix)
    echo -e "${YELLOW}Fix mode enabled - will apply suggested fixes${NC}"
fi

# Find files to lint
if [ -n "$FILE_PATTERN" ]; then
    FILES="$FILE_PATTERN"
else
    FILES=$(find src -type f -name "*.cpp" 2>/dev/null | sort)
fi

FILE_COUNT=$(echo "$FILES" | wc -w)
echo -e "${YELLOW}Linting $FILE_COUNT file(s)...${NC}"
echo ""

ISSUES_FOUND=0

for file in $FILES; do
    if [ "$VERBOSE" = true ]; then
        echo -e "${BLUE}Checking: $file${NC}"
    fi

    if ! clang-tidy "${TIDY_ARGS[@]}" "$file" 2>/dev/null; then
        ISSUES_FOUND=$((ISSUES_FOUND + 1))
    fi
done

echo ""
if [ $ISSUES_FOUND -eq 0 ]; then
    echo -e "${GREEN}No issues found!${NC}"
else
    echo -e "${YELLOW}Found issues in $ISSUES_FOUND file(s)${NC}"
    if [ "$FIX" = false ]; then
        echo -e "${YELLOW}Run with -f to apply fixes${NC}"
    fi
fi

echo -e "${BLUE}=====================================${NC}"
