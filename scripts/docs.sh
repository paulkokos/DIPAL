#!/bin/bash
# DIPAL Documentation Generator
# Usage: ./scripts/docs.sh [options]

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

OUTPUT_DIR="docs/api"
OPEN_DOCS=false

usage() {
    echo "DIPAL Documentation Generator"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -o, --open      Open documentation in browser after generation"
    echo "  -c, --clean     Clean docs before regenerating"
    echo "  -h, --help      Show this help message"
    echo ""
    echo "Requirements: doxygen, graphviz (optional, for diagrams)"
}

CLEAN=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -o|--open)
            OPEN_DOCS=true
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
echo -e "${BLUE}  DIPAL Documentation Generator${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""

cd "$PROJECT_ROOT"

# Check for doxygen
if ! command -v doxygen &> /dev/null; then
    echo -e "${RED}Error: doxygen not found${NC}"
    echo "Please install doxygen:"
    echo "  Ubuntu/Debian: sudo apt install doxygen graphviz"
    echo "  macOS: brew install doxygen graphviz"
    exit 1
fi

# Create Doxyfile if it doesn't exist
if [ ! -f "Doxyfile" ]; then
    echo -e "${YELLOW}Creating Doxyfile...${NC}"
    DIPAL_VERSION=$(grep -E 'VERSION [0-9]+\.[0-9]+\.[0-9]+' CMakeLists.txt | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1)
    cat > Doxyfile << EOF
# DIPAL Doxygen Configuration

PROJECT_NAME           = "DIPAL"
PROJECT_BRIEF          = "Digital Image Processing and Analysis Library"
PROJECT_NUMBER         = ${DIPAL_VERSION}

OUTPUT_DIRECTORY       = docs/api
INPUT                  = include src
RECURSIVE              = YES
FILE_PATTERNS          = *.hpp *.cpp *.h

EXTRACT_ALL            = YES
EXTRACT_PRIVATE        = NO
EXTRACT_STATIC         = YES

GENERATE_HTML          = YES
GENERATE_LATEX         = NO
GENERATE_XML           = NO

HTML_OUTPUT            = html
HTML_COLORSTYLE_HUE    = 220
HTML_COLORSTYLE_SAT    = 100
HTML_COLORSTYLE_GAMMA  = 80
HTML_DYNAMIC_SECTIONS  = YES

HAVE_DOT               = YES
DOT_IMAGE_FORMAT       = svg
INTERACTIVE_SVG        = YES
CALL_GRAPH             = YES
CALLER_GRAPH           = YES
CLASS_GRAPH            = YES
COLLABORATION_GRAPH    = YES
INCLUDE_GRAPH          = YES
INCLUDED_BY_GRAPH      = YES

QUIET                  = YES
WARNINGS               = YES
WARN_IF_UNDOCUMENTED   = NO
WARN_IF_DOC_ERROR      = YES

USE_MDFILE_AS_MAINPAGE = README.md
EOF
fi

# Clean if requested
if [ "$CLEAN" = true ]; then
    echo -e "${YELLOW}Cleaning documentation...${NC}"
    rm -rf "$OUTPUT_DIR"
fi

# Generate documentation
echo -e "${YELLOW}Generating documentation...${NC}"
mkdir -p "$OUTPUT_DIR"
doxygen Doxyfile

echo ""
echo -e "${GREEN}Documentation generated!${NC}"
echo -e "  Output: ${BLUE}$OUTPUT_DIR/html/index.html${NC}"

# Open if requested
if [ "$OPEN_DOCS" = true ]; then
    if command -v xdg-open &> /dev/null; then
        xdg-open "$OUTPUT_DIR/html/index.html"
    elif command -v open &> /dev/null; then
        open "$OUTPUT_DIR/html/index.html"
    fi
fi

echo -e "${BLUE}=====================================${NC}"
