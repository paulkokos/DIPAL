#!/bin/bash
# DIPAL Cppcheck Static Analysis
# Usage: ./scripts/cppcheck.sh [options]

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

OUTPUT_DIR="cppcheck-reports"
HTML_REPORT=false
SEVERITY="warning"

usage() {
    echo "DIPAL Cppcheck Static Analysis"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -s, --severity LEVEL   Minimum severity: error, warning, style, performance, all"
    echo "                         (default: warning)"
    echo "  --html                 Generate HTML report"
    echo "  -o, --output DIR       Output directory (default: cppcheck-reports/)"
    echo "  -h, --help             Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                     # Run with default settings"
    echo "  $0 -s error            # Only show errors"
    echo "  $0 --html              # Generate HTML report"
}

while [[ $# -gt 0 ]]; do
    case $1 in
        -s|--severity)
            SEVERITY="$2"
            shift 2
            ;;
        --html)
            HTML_REPORT=true
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
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            usage
            exit 1
            ;;
    esac
done

echo -e "${BLUE}=====================================${NC}"
echo -e "${BLUE}  DIPAL Cppcheck Analysis${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""

cd "$PROJECT_ROOT"

# Check for cppcheck
if ! command -v cppcheck &> /dev/null; then
    echo -e "${RED}Error: cppcheck not found${NC}"
    echo "Please install cppcheck:"
    echo "  Ubuntu/Debian: sudo apt install cppcheck cppcheck-htmlreport"
    echo "  macOS: brew install cppcheck"
    exit 1
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Build cppcheck arguments
CPPCHECK_ARGS=(
    --enable=all
    --std=c++23
    --language=c++
    --platform=unix64
    --suppress=missingIncludeSystem
    --suppress=unmatchedSuppression
    -I include
    src
)

# Set severity
case $SEVERITY in
    error)
        CPPCHECK_ARGS+=(--error-exitcode=1)
        ;;
    warning)
        CPPCHECK_ARGS+=(--enable=warning,performance,portability)
        ;;
    style)
        CPPCHECK_ARGS+=(--enable=style)
        ;;
    performance)
        CPPCHECK_ARGS+=(--enable=performance)
        ;;
    all)
        CPPCHECK_ARGS+=(--enable=all)
        ;;
esac

TIMESTAMP=$(date +%Y%m%d_%H%M%S)
REPORT_FILE="$OUTPUT_DIR/cppcheck_${TIMESTAMP}.txt"
XML_FILE="$OUTPUT_DIR/cppcheck_${TIMESTAMP}.xml"

echo -e "${YELLOW}Running cppcheck (severity: $SEVERITY)...${NC}"
echo ""

if [ "$HTML_REPORT" = true ]; then
    # Generate XML for HTML report
    cppcheck "${CPPCHECK_ARGS[@]}" --xml 2> "$XML_FILE"

    # Generate HTML report
    if command -v cppcheck-htmlreport &> /dev/null; then
        cppcheck-htmlreport --file="$XML_FILE" --report-dir="$OUTPUT_DIR/html" --source-dir=.
        echo -e "${GREEN}HTML report generated: $OUTPUT_DIR/html/index.html${NC}"
    else
        echo -e "${YELLOW}cppcheck-htmlreport not found, XML report saved${NC}"
    fi
fi

# Also generate text report
cppcheck "${CPPCHECK_ARGS[@]}" 2>&1 | tee "$REPORT_FILE"

# Count issues
ERROR_COUNT=$(grep -c "error:" "$REPORT_FILE" 2>/dev/null || echo "0")
WARNING_COUNT=$(grep -c "warning:" "$REPORT_FILE" 2>/dev/null || echo "0")
STYLE_COUNT=$(grep -c "style:" "$REPORT_FILE" 2>/dev/null || echo "0")
PERF_COUNT=$(grep -c "performance:" "$REPORT_FILE" 2>/dev/null || echo "0")

echo ""
echo -e "${BLUE}=====================================${NC}"
echo -e "  ${RED}Errors:${NC}      $ERROR_COUNT"
echo -e "  ${YELLOW}Warnings:${NC}    $WARNING_COUNT"
echo -e "  ${BLUE}Style:${NC}       $STYLE_COUNT"
echo -e "  ${GREEN}Performance:${NC} $PERF_COUNT"
echo -e "${BLUE}=====================================${NC}"
echo ""
echo -e "Report saved: ${BLUE}$REPORT_FILE${NC}"

if [ "$ERROR_COUNT" -gt 0 ]; then
    exit 1
fi
