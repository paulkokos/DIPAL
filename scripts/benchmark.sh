#!/bin/bash
# DIPAL Benchmark Script
# Usage: ./scripts/benchmark.sh [options]

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

BUILD_DIR="build-bench"
OUTPUT_FILE=""
ITERATIONS=5
COMPARE_BASELINE=false
BASELINE_FILE="benchmarks/baseline.json"

usage() {
    echo "DIPAL Benchmark Script"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -i, --iterations N   Number of iterations (default: 5)"
    echo "  -o, --output FILE    Save results to JSON file"
    echo "  -b, --baseline       Compare against baseline"
    echo "  --save-baseline      Save current results as baseline"
    echo "  -c, --clean          Clean build before benchmarking"
    echo "  -h, --help           Show this help message"
    echo ""
    echo "This script builds with optimizations and runs performance tests."
}

CLEAN=false
SAVE_BASELINE=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -i|--iterations)
            ITERATIONS="$2"
            shift 2
            ;;
        -o|--output)
            OUTPUT_FILE="$2"
            shift 2
            ;;
        -b|--baseline)
            COMPARE_BASELINE=true
            shift
            ;;
        --save-baseline)
            SAVE_BASELINE=true
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

echo -e "${BLUE}=====================================${NC}"
echo -e "${BLUE}  DIPAL Benchmarks${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""

cd "$PROJECT_ROOT"

# Clean if requested
if [ "$CLEAN" = true ]; then
    rm -rf "$BUILD_DIR"
fi

# Build with full optimizations
echo -e "${YELLOW}Building with optimizations...${NC}"
mkdir -p "$BUILD_DIR"

cmake -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS="-O3 -DNDEBUG -march=native" \
    -DBUILD_TESTS=ON \
    -DBUILD_EXAMPLES=OFF

cmake --build "$BUILD_DIR" --parallel

# Create benchmarks directory
mkdir -p benchmarks

# Run benchmarks
echo ""
echo -e "${YELLOW}Running benchmarks ($ITERATIONS iterations)...${NC}"
echo ""

# Results file
RESULTS_FILE="benchmarks/results_$(date +%Y%m%d_%H%M%S).txt"

{
    echo "DIPAL Benchmark Results"
    echo "======================="
    echo "Date: $(date)"
    echo "Iterations: $ITERATIONS"
    echo "System: $(uname -a)"
    echo ""

    # Run performance tests if they exist
    if [ -f "$BUILD_DIR/tests/filter_performance_tests" ]; then
        echo "=== Filter Performance ==="
        for i in $(seq 1 $ITERATIONS); do
            echo "--- Iteration $i ---"
            "$BUILD_DIR/tests/filter_performance_tests" --gtest_filter="*Performance*" 2>&1 || true
        done
        echo ""
    fi

    if [ -f "$BUILD_DIR/tests/large_image_tests" ]; then
        echo "=== Large Image Performance ==="
        for i in $(seq 1 $ITERATIONS); do
            echo "--- Iteration $i ---"
            "$BUILD_DIR/tests/large_image_tests" --gtest_filter="*Performance*" 2>&1 || true
        done
        echo ""
    fi

    # Run all performance labeled tests
    echo "=== All Performance Tests ==="
    ctest --test-dir "$BUILD_DIR" -L performance --output-on-failure 2>&1 || true

} | tee "$RESULTS_FILE"

echo ""
echo -e "${GREEN}Benchmark complete!${NC}"
echo -e "  Results saved to: ${BLUE}$RESULTS_FILE${NC}"

# Save as baseline if requested
if [ "$SAVE_BASELINE" = true ]; then
    cp "$RESULTS_FILE" "$BASELINE_FILE"
    echo -e "  Saved as baseline: ${BLUE}$BASELINE_FILE${NC}"
fi

# Compare with baseline if requested
if [ "$COMPARE_BASELINE" = true ] && [ -f "$BASELINE_FILE" ]; then
    echo ""
    echo -e "${YELLOW}Comparing with baseline...${NC}"
    diff -u "$BASELINE_FILE" "$RESULTS_FILE" || true
fi

# Save to specified output file
if [ -n "$OUTPUT_FILE" ]; then
    cp "$RESULTS_FILE" "$OUTPUT_FILE"
    echo -e "  Copied to: ${BLUE}$OUTPUT_FILE${NC}"
fi

echo -e "${BLUE}=====================================${NC}"
