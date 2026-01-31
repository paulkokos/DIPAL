#!/bin/bash
# DIPAL Sanitizer Build Script
# Usage: ./scripts/sanitize.sh [options]

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

# Default values
SANITIZER="address"
BUILD_DIR="build-sanitize"
RUN_TESTS=true
PARALLEL_JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

usage() {
    echo "DIPAL Sanitizer Build Script"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Sanitizer options:"
    echo "  -a, --address     AddressSanitizer (default) - memory errors"
    echo "  -u, --undefined   UndefinedBehaviorSanitizer - undefined behavior"
    echo "  -t, --thread      ThreadSanitizer - data races"
    echo "  -m, --memory      MemorySanitizer - uninitialized reads (Clang only)"
    echo "  --all             Run all sanitizers sequentially"
    echo ""
    echo "Other options:"
    echo "  --no-test         Don't run tests after building"
    echo "  -c, --clean       Clean build before building"
    echo "  -h, --help        Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                # Build with AddressSanitizer and run tests"
    echo "  $0 -u             # Build with UBSan"
    echo "  $0 --all          # Run all sanitizers"
}

CLEAN=false
RUN_ALL=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -a|--address)
            SANITIZER="address"
            shift
            ;;
        -u|--undefined)
            SANITIZER="undefined"
            shift
            ;;
        -t|--thread)
            SANITIZER="thread"
            shift
            ;;
        -m|--memory)
            SANITIZER="memory"
            shift
            ;;
        --all)
            RUN_ALL=true
            shift
            ;;
        --no-test)
            RUN_TESTS=false
            shift
            ;;
        -c|--clean)
            CLEAN=true
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

run_sanitizer() {
    local san="$1"
    local san_flags=""
    local san_name=""

    case $san in
        address)
            san_flags="-fsanitize=address -fno-omit-frame-pointer"
            san_name="AddressSanitizer"
            ;;
        undefined)
            san_flags="-fsanitize=undefined -fno-omit-frame-pointer"
            san_name="UndefinedBehaviorSanitizer"
            ;;
        thread)
            san_flags="-fsanitize=thread"
            san_name="ThreadSanitizer"
            ;;
        memory)
            san_flags="-fsanitize=memory -fno-omit-frame-pointer"
            san_name="MemorySanitizer"
            ;;
    esac

    local build_dir="build-$san"

    echo -e "${BLUE}=====================================${NC}"
    echo -e "${BLUE}  $san_name${NC}"
    echo -e "${BLUE}=====================================${NC}"
    echo ""

    cd "$PROJECT_ROOT"

    if [ "$CLEAN" = true ]; then
        rm -rf "$build_dir"
    fi

    mkdir -p "$build_dir"

    echo -e "${YELLOW}Configuring with $san_name...${NC}"
    cmake -B "$build_dir" \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_CXX_FLAGS="$san_flags" \
        -DCMAKE_C_FLAGS="$san_flags" \
        -DCMAKE_EXE_LINKER_FLAGS="$san_flags" \
        -DCMAKE_SHARED_LINKER_FLAGS="$san_flags" \
        -DBUILD_TESTS=ON \
        -DBUILD_EXAMPLES=OFF

    echo -e "${YELLOW}Building...${NC}"
    cmake --build "$build_dir" --parallel "$PARALLEL_JOBS"

    if [ "$RUN_TESTS" = true ]; then
        echo -e "${YELLOW}Running tests with $san_name...${NC}"

        # Set sanitizer options
        export ASAN_OPTIONS="detect_leaks=1:abort_on_error=1"
        export UBSAN_OPTIONS="print_stacktrace=1:abort_on_error=1"
        export TSAN_OPTIONS="second_deadlock_stack=1"
        export MSAN_OPTIONS="abort_on_error=1"

        if ctest --test-dir "$build_dir" --output-on-failure; then
            echo -e "${GREEN}All tests passed with $san_name!${NC}"
        else
            echo -e "${RED}Tests failed with $san_name!${NC}"
            return 1
        fi
    fi

    echo ""
}

echo -e "${BLUE}=====================================${NC}"
echo -e "${BLUE}  DIPAL Sanitizer Build${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""

cd "$PROJECT_ROOT"

if [ "$RUN_ALL" = true ]; then
    FAILED=0

    for san in address undefined thread; do
        if ! run_sanitizer "$san"; then
            FAILED=$((FAILED + 1))
        fi
        echo ""
    done

    echo -e "${BLUE}=====================================${NC}"
    if [ $FAILED -eq 0 ]; then
        echo -e "${GREEN}All sanitizers passed!${NC}"
    else
        echo -e "${RED}$FAILED sanitizer(s) found issues${NC}"
        exit 1
    fi
else
    run_sanitizer "$SANITIZER"
fi

echo -e "${BLUE}=====================================${NC}"
