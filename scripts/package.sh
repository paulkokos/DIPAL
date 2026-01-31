#!/bin/bash
# DIPAL Package Script
# Usage: ./scripts/package.sh [options]

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

# Get version from CMakeLists.txt
VERSION=$(grep -oP 'VERSION \K[0-9]+\.[0-9]+\.[0-9]+' "$PROJECT_ROOT/CMakeLists.txt" | head -1)
if [ -z "$VERSION" ]; then
    VERSION="0.0.0"
fi

OUTPUT_DIR="dist"
BUILD_RELEASE=true
INCLUDE_SOURCE=false

usage() {
    echo "DIPAL Package Script"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -v, --version VER   Override version (default: $VERSION from CMakeLists.txt)"
    echo "  -o, --output DIR    Output directory (default: dist/)"
    echo "  -s, --source        Include source archive"
    echo "  --no-build          Skip building (use existing build)"
    echo "  -h, --help          Show this help message"
    echo ""
    echo "Creates:"
    echo "  - Binary release archive (tar.gz/zip)"
    echo "  - Source archive (optional)"
    echo "  - Checksums file"
}

while [[ $# -gt 0 ]]; do
    case $1 in
        -v|--version)
            VERSION="$2"
            shift 2
            ;;
        -o|--output)
            OUTPUT_DIR="$2"
            shift 2
            ;;
        -s|--source)
            INCLUDE_SOURCE=true
            shift
            ;;
        --no-build)
            BUILD_RELEASE=false
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
echo -e "${BLUE}  DIPAL Package Creator${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""
echo -e "  Version: ${GREEN}$VERSION${NC}"
echo -e "  Output:  ${GREEN}$OUTPUT_DIR${NC}"
echo ""

cd "$PROJECT_ROOT"

# Detect platform
PLATFORM=$(uname -s | tr '[:upper:]' '[:lower:]')
ARCH=$(uname -m)

case $PLATFORM in
    linux)
        PLATFORM_NAME="linux"
        ARCHIVE_EXT="tar.gz"
        ;;
    darwin)
        PLATFORM_NAME="macos"
        ARCHIVE_EXT="tar.gz"
        ;;
    mingw*|msys*|cygwin*)
        PLATFORM_NAME="windows"
        ARCHIVE_EXT="zip"
        ;;
    *)
        PLATFORM_NAME="$PLATFORM"
        ARCHIVE_EXT="tar.gz"
        ;;
esac

# Build if requested
if [ "$BUILD_RELEASE" = true ]; then
    echo -e "${YELLOW}Building release...${NC}"
    "$SCRIPT_DIR/build.sh" --clean --no-tests --no-examples
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Package name
PACKAGE_NAME="DIPAL-${VERSION}-${PLATFORM_NAME}-${ARCH}"

echo -e "${YELLOW}Creating package: $PACKAGE_NAME${NC}"

# Create staging directory
STAGING_DIR="$OUTPUT_DIR/staging/$PACKAGE_NAME"
rm -rf "$OUTPUT_DIR/staging"
mkdir -p "$STAGING_DIR"

# Copy installed files
cmake --install build --prefix "$STAGING_DIR"

# Add README and LICENSE
cp README.md "$STAGING_DIR/" 2>/dev/null || true
cp LICENSE "$STAGING_DIR/" 2>/dev/null || true

# Create archive
cd "$OUTPUT_DIR/staging"
if [ "$ARCHIVE_EXT" = "zip" ]; then
    zip -r "../$PACKAGE_NAME.zip" "$PACKAGE_NAME"
    ARCHIVE_FILE="$PACKAGE_NAME.zip"
else
    tar -czvf "../$PACKAGE_NAME.tar.gz" "$PACKAGE_NAME"
    ARCHIVE_FILE="$PACKAGE_NAME.tar.gz"
fi
cd "$PROJECT_ROOT"

# Create source archive if requested
if [ "$INCLUDE_SOURCE" = true ]; then
    echo -e "${YELLOW}Creating source archive...${NC}"
    SOURCE_NAME="DIPAL-${VERSION}-source"

    git archive --format=tar.gz --prefix="$SOURCE_NAME/" HEAD > "$OUTPUT_DIR/$SOURCE_NAME.tar.gz"
fi

# Generate checksums
echo -e "${YELLOW}Generating checksums...${NC}"
cd "$OUTPUT_DIR"
{
    echo "DIPAL $VERSION Checksums"
    echo "========================"
    echo ""
    echo "SHA256:"
    sha256sum *.tar.gz *.zip 2>/dev/null || true
    echo ""
    echo "SHA512:"
    sha512sum *.tar.gz *.zip 2>/dev/null || true
} > "CHECKSUMS-$VERSION.txt"
cd "$PROJECT_ROOT"

# Cleanup staging
rm -rf "$OUTPUT_DIR/staging"

echo ""
echo -e "${GREEN}Package created successfully!${NC}"
echo ""
echo -e "Files in ${BLUE}$OUTPUT_DIR/${NC}:"
ls -lh "$OUTPUT_DIR"/*.{tar.gz,zip,txt} 2>/dev/null | awk '{print "  " $NF " (" $5 ")"}'

echo ""
echo -e "${BLUE}=====================================${NC}"
