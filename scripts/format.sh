#!/bin/bash
# DIPAL Format Script
# Usage: ./scripts/format.sh [options]

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

CHECK_ONLY=false
VERBOSE=false

usage() {
    echo "DIPAL Format Script"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -c, --check     Check formatting without modifying files"
    echo "  -v, --verbose   Show files being processed"
    echo "  -h, --help      Show this help message"
    echo ""
    echo "This script formats all .cpp and .hpp files in src/ and include/"
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -c|--check)
            CHECK_ONLY=true
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
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            usage
            exit 1
            ;;
    esac
done

echo -e "${BLUE}=====================================${NC}"
echo -e "${BLUE}  DIPAL Code Formatter${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""

cd "$PROJECT_ROOT"

# Check if clang-format is available
if ! command -v clang-format &> /dev/null; then
    echo -e "${RED}Error: clang-format not found${NC}"
    echo "Please install clang-format:"
    echo "  Ubuntu/Debian: sudo apt install clang-format"
    echo "  macOS: brew install clang-format"
    exit 1
fi

# Find all source files
FILES=$(find src include -type f \( -name "*.cpp" -o -name "*.hpp" \) 2>/dev/null | sort)
FILE_COUNT=$(echo "$FILES" | wc -l)

echo -e "${YELLOW}Found $FILE_COUNT source files${NC}"
echo ""

if [ "$CHECK_ONLY" = true ]; then
    echo -e "${YELLOW}Checking formatting...${NC}"
    FAILED=0

    for file in $FILES; do
        if [ "$VERBOSE" = true ]; then
            echo "  Checking: $file"
        fi

        if ! clang-format --dry-run --Werror "$file" 2>/dev/null; then
            echo -e "${RED}  Needs formatting: $file${NC}"
            FAILED=$((FAILED + 1))
        fi
    done

    echo ""
    if [ $FAILED -eq 0 ]; then
        echo -e "${GREEN}All files are properly formatted!${NC}"
    else
        echo -e "${RED}$FAILED file(s) need formatting.${NC}"
        echo -e "${YELLOW}Run './scripts/format.sh' to fix.${NC}"
        exit 1
    fi
else
    echo -e "${YELLOW}Formatting files...${NC}"

    for file in $FILES; do
        if [ "$VERBOSE" = true ]; then
            echo "  Formatting: $file"
        fi
        clang-format -i "$file"
    done

    echo ""
    echo -e "${GREEN}Formatting complete!${NC}"
fi

echo -e "${BLUE}=====================================${NC}"
