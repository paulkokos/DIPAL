#!/bin/bash
# DIPAL Valgrind Memory Check Script
# Usage: ./scripts/valgrind.sh [options]

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

BUILD_DIR="build-debug"
OUTPUT_DIR="valgrind-reports"
CHECK_TYPE="memcheck"

usage() {
    echo "DIPAL Valgrind Memory Check"
    echo ""
    echo "Usage: $0 [options] [test-filter]"
    echo ""
    echo "Check types:"
    echo "  -m, --memcheck      Memory error detection (default)"
    echo "  -c, --cachegrind    Cache profiling"
    echo "  -l, --callgrind     Call profiling"
    echo "  -e, --helgrind      Thread error detection"
    echo ""
    echo "Other options:"
    echo "  -b, --build         Build before running"
    echo "  -o, --output DIR    Output directory (default: valgrind-reports/)"
    echo "  -h, --help          Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                  # Run memcheck on all tests"
    echo "  $0 -b               # Build debug and run memcheck"
    echo "  $0 -c               # Run cachegrind"
}

BUILD_FIRST=false
TEST_FILTER=""

while [[ $# -gt 0 ]]; do
    case $1 in
        -m|--memcheck)
            CHECK_TYPE="memcheck"
            shift
            ;;
        -c|--cachegrind)
            CHECK_TYPE="cachegrind"
            shift
            ;;
        -l|--callgrind)
            CHECK_TYPE="callgrind"
            shift
            ;;
        -e|--helgrind)
            CHECK_TYPE="helgrind"
            shift
            ;;
        -b|--build)
            BUILD_FIRST=true
            shift
            ;;
        -o|--output)
            OUTPUT_DIR="$2"
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
            TEST_FILTER="$1"
            shift
            ;;
    esac
done

echo -e "${BLUE}=====================================${NC}"
echo -e "${BLUE}  DIPAL Valgrind Analysis${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""

cd "$PROJECT_ROOT"

# Check for valgrind
if ! command -v valgrind &> /dev/null; then
    echo -e "${RED}Error: valgrind not found${NC}"
    echo "Please install valgrind:"
    echo "  Ubuntu/Debian: sudo apt install valgrind"
    echo "  macOS: brew install valgrind (limited support)"
    exit 1
fi

# Build if requested
if [ "$BUILD_FIRST" = true ]; then
    echo -e "${YELLOW}Building debug version...${NC}"
    "$SCRIPT_DIR/build.sh" -d
    echo ""
fi

# Check build directory
if [ ! -d "$BUILD_DIR" ]; then
    echo -e "${RED}Build directory not found. Run with -b to build first.${NC}"
    exit 1
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Timestamp for this run
TIMESTAMP=$(date +%Y%m%d_%H%M%S)

echo -e "${YELLOW}Running $CHECK_TYPE...${NC}"
echo ""

# Find test executables
TEST_EXECS=$(find "$BUILD_DIR/tests" -type f -executable 2>/dev/null || true)

if [ -z "$TEST_EXECS" ]; then
    echo -e "${RED}No test executables found in $BUILD_DIR/tests${NC}"
    exit 1
fi

REPORT_FILE="$OUTPUT_DIR/${CHECK_TYPE}_${TIMESTAMP}.txt"

{
    echo "DIPAL Valgrind Report"
    echo "====================="
    echo "Type: $CHECK_TYPE"
    echo "Date: $(date)"
    echo ""

    for test_exec in $TEST_EXECS; do
        test_name=$(basename "$test_exec")

        # Skip if filter doesn't match
        if [ -n "$TEST_FILTER" ] && [[ ! "$test_name" == *"$TEST_FILTER"* ]]; then
            continue
        fi

        echo "=== $test_name ==="
        echo ""

        case $CHECK_TYPE in
            memcheck)
                valgrind --tool=memcheck \
                    --leak-check=full \
                    --show-leak-kinds=all \
                    --track-origins=yes \
                    --verbose \
                    "$test_exec" 2>&1 || true
                ;;
            cachegrind)
                valgrind --tool=cachegrind \
                    --cachegrind-out-file="$OUTPUT_DIR/cachegrind.out.$test_name" \
                    "$test_exec" 2>&1 || true
                ;;
            callgrind)
                valgrind --tool=callgrind \
                    --callgrind-out-file="$OUTPUT_DIR/callgrind.out.$test_name" \
                    "$test_exec" 2>&1 || true
                ;;
            helgrind)
                valgrind --tool=helgrind \
                    "$test_exec" 2>&1 || true
                ;;
        esac

        echo ""
        echo ""
    done

} | tee "$REPORT_FILE"

echo ""
echo -e "${GREEN}Valgrind analysis complete!${NC}"
echo -e "  Report: ${BLUE}$REPORT_FILE${NC}"

# Show summary for memcheck
if [ "$CHECK_TYPE" = "memcheck" ]; then
    LEAKS=$(grep -c "definitely lost" "$REPORT_FILE" 2>/dev/null || echo "0")
    ERRORS=$(grep -c "ERROR SUMMARY" "$REPORT_FILE" 2>/dev/null || echo "0")
    echo ""
    echo -e "  Potential leaks found: ${YELLOW}$LEAKS${NC}"
fi

echo -e "${BLUE}=====================================${NC}"
