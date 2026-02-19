#!/bin/bash
# DIPAL Include-What-You-Use (IWYU) Script
# Usage: ./scripts/iwyu.sh [options]

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
OUTPUT_FILE=""

usage() {
    echo "DIPAL Include-What-You-Use (IWYU) Script"
    echo ""
    echo "Usage: $0 [options] [file]"
    echo ""
    echo "Options:"
    echo "  -f, --fix         Apply suggested fixes"
    echo "  -o, --output FILE Save report to file"
    echo "  -h, --help        Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                      # Check all source files"
    echo "  $0 src/Image/Image.cpp  # Check specific file"
    echo "  $0 -f                   # Check and fix all files"
    echo ""
    echo "Requirements: include-what-you-use"
    echo "  Ubuntu: sudo apt install iwyu"
    echo "  macOS:  brew install include-what-you-use"
}

TARGET_FILE=""

while [[ $# -gt 0 ]]; do
    case $1 in
        -f|--fix)
            FIX=true
            shift
            ;;
        -o|--output)
            OUTPUT_FILE="$2"
            shift 2
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
            TARGET_FILE="$1"
            shift
            ;;
    esac
done

echo -e "${BLUE}=====================================${NC}"
echo -e "${BLUE}  DIPAL Include-What-You-Use${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""

cd "$PROJECT_ROOT"

# Check for iwyu
if ! command -v include-what-you-use &> /dev/null; then
    echo -e "${RED}Error: include-what-you-use not found${NC}"
    echo ""
    echo "Please install IWYU:"
    echo "  Ubuntu/Debian: sudo apt install iwyu"
    echo "  macOS: brew install include-what-you-use"
    echo "  From source: https://github.com/include-what-you-use/include-what-you-use"
    exit 1
fi

# Check for compile_commands.json
COMPILE_DB="$BUILD_DIR/compile_commands.json"
if [ ! -f "$COMPILE_DB" ] && [ ! -f "compile_commands.json" ]; then
    echo -e "${YELLOW}compile_commands.json not found. Running configure...${NC}"
    "$SCRIPT_DIR/configure.sh"
fi

if [ -f "compile_commands.json" ]; then
    COMPILE_DB="compile_commands.json"
fi

echo -e "${YELLOW}Using: $COMPILE_DB${NC}"
echo ""

# Create reports directory
mkdir -p iwyu-reports
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
REPORT_FILE="iwyu-reports/iwyu_${TIMESTAMP}.txt"

# Find files to check
if [ -n "$TARGET_FILE" ]; then
    FILES="$TARGET_FILE"
else
    FILES=$(find src -name "*.cpp" | sort)
fi

FILE_COUNT=$(echo "$FILES" | wc -w)
echo -e "${YELLOW}Checking $FILE_COUNT file(s)...${NC}"
echo ""

# Run IWYU
{
    echo "DIPAL IWYU Report"
    echo "================="
    echo "Date: $(date)"
    echo ""

    for file in $FILES; do
        echo "=== $file ==="

        # Get compile command for this file
        if command -v jq &> /dev/null; then
            COMPILE_CMD=$(jq -r ".[] | select(.file | endswith(\"$file\")) | .command" "$COMPILE_DB" 2>/dev/null | head -1)
        fi

        # Run IWYU
        include-what-you-use \
            -Xiwyu --no_fwd_decls \
            -Xiwyu --cxx17ns \
            -Xiwyu --max_line_length=100 \
            -p "$BUILD_DIR" \
            "$file" 2>&1 || true

        echo ""
    done

} | tee "$REPORT_FILE"

# Apply fixes if requested
if [ "$FIX" = true ]; then
    echo ""
    echo -e "${YELLOW}Applying fixes...${NC}"

    if command -v fix_includes.py &> /dev/null; then
        fix_includes.py < "$REPORT_FILE"
        echo -e "${GREEN}Fixes applied!${NC}"
    elif command -v iwyu-fix-includes &> /dev/null; then
        iwyu-fix-includes < "$REPORT_FILE"
        echo -e "${GREEN}Fixes applied!${NC}"
    else
        echo -e "${RED}fix_includes.py not found. Install with:${NC}"
        echo "  pip install iwyu"
        echo ""
        echo "Or manually review: $REPORT_FILE"
    fi
fi

# Copy to output file if specified
if [ -n "$OUTPUT_FILE" ]; then
    cp "$REPORT_FILE" "$OUTPUT_FILE"
fi

echo ""
echo -e "${GREEN}IWYU analysis complete!${NC}"
echo -e "  Report: ${BLUE}$REPORT_FILE${NC}"
echo ""
echo -e "${YELLOW}Note: IWYU suggestions are recommendations, not all need to be applied.${NC}"
echo -e "${YELLOW}Review carefully before applying fixes.${NC}"
echo -e "${BLUE}=====================================${NC}"
