#!/bin/bash
# DIPAL Version Management Script
# Usage: ./scripts/version.sh [options]

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

usage() {
    echo "DIPAL Version Management"
    echo ""
    echo "Usage: $0 [command] [options]"
    echo ""
    echo "Commands:"
    echo "  show              Show current version"
    echo "  major             Bump major version (x.0.0)"
    echo "  minor             Bump minor version (0.x.0)"
    echo "  patch             Bump patch version (0.0.x)"
    echo "  set VERSION       Set specific version"
    echo ""
    echo "Options:"
    echo "  --dry-run         Show what would be changed without modifying"
    echo "  --tag             Create git tag after version bump"
    echo "  -h, --help        Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 show           # Show current version"
    echo "  $0 patch          # 0.1.4 -> 0.1.5"
    echo "  $0 minor          # 0.1.4 -> 0.2.0"
    echo "  $0 set 1.0.0      # Set to 1.0.0"
}

DRY_RUN=false
CREATE_TAG=false
COMMAND=""
NEW_VERSION=""

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        show|major|minor|patch)
            COMMAND="$1"
            shift
            ;;
        set)
            COMMAND="set"
            NEW_VERSION="$2"
            shift 2
            ;;
        --dry-run)
            DRY_RUN=true
            shift
            ;;
        --tag)
            CREATE_TAG=true
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

if [ -z "$COMMAND" ]; then
    usage
    exit 1
fi

cd "$PROJECT_ROOT"

# Portable in-place sed (works on both BSD/macOS and GNU/Linux sed)
portable_sed() {
    local expr="$1"
    local file="$2"
    sed -i.bak -E "$expr" "$file" && rm -f "$file.bak"
}

# Get current version from CMakeLists.txt (portable: no grep -P)
get_version() {
    grep -E 'VERSION [0-9]+\.[0-9]+\.[0-9]+' CMakeLists.txt | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1
}

# Set version in all files
set_version() {
    local new_ver="$1"
    local major=$(echo "$new_ver" | cut -d. -f1)
    local minor=$(echo "$new_ver" | cut -d. -f2)
    local patch=$(echo "$new_ver" | cut -d. -f3)

    echo -e "${YELLOW}Updating version to $new_ver...${NC}"

    if [ "$DRY_RUN" = true ]; then
        echo "  Would update: CMakeLists.txt"
        echo "  Would update: include/DIPAL/DIPAL.hpp (if version macros exist)"
        echo "  Would update: Doxyfile (if exists)"
        return
    fi

    # Update CMakeLists.txt
    portable_sed "s/VERSION [0-9]+\.[0-9]+\.[0-9]+/VERSION $new_ver/" CMakeLists.txt
    echo -e "  ${GREEN}Updated: CMakeLists.txt${NC}"

    # Update DIPAL.hpp version macros if they exist
    if [ -f "include/DIPAL/DIPAL.hpp" ]; then
        if grep -q "DIPAL_VERSION_MAJOR" include/DIPAL/DIPAL.hpp; then
            portable_sed "s/#define DIPAL_VERSION_MAJOR [0-9]+/#define DIPAL_VERSION_MAJOR $major/" include/DIPAL/DIPAL.hpp
            portable_sed "s/#define DIPAL_VERSION_MINOR [0-9]+/#define DIPAL_VERSION_MINOR $minor/" include/DIPAL/DIPAL.hpp
            portable_sed "s/#define DIPAL_VERSION_PATCH [0-9]+/#define DIPAL_VERSION_PATCH $patch/" include/DIPAL/DIPAL.hpp
            echo -e "  ${GREEN}Updated: include/DIPAL/DIPAL.hpp${NC}"
        fi
    fi

    # Update Doxyfile if it exists
    if [ -f "Doxyfile" ]; then
        portable_sed "s/PROJECT_NUMBER *= *[0-9]+\.[0-9]+\.[0-9]+/PROJECT_NUMBER         = $new_ver/" Doxyfile
        echo -e "  ${GREEN}Updated: Doxyfile${NC}"
    fi

    # Update packaging files
    for file in packaging/vcpkg/vcpkg.json packaging/conan/conanfile.py packaging/homebrew/dipal.rb packaging/aur/PKGBUILD; do
        if [ -f "$file" ]; then
            portable_sed "s/[0-9]+\.[0-9]+\.[0-9]+/$new_ver/g" "$file"
            echo -e "  ${GREEN}Updated: $file${NC}"
        fi
    done
}

CURRENT_VERSION=$(get_version)

case $COMMAND in
    show)
        echo -e "Current version: ${GREEN}$CURRENT_VERSION${NC}"
        exit 0
        ;;
    major)
        MAJOR=$(echo "$CURRENT_VERSION" | cut -d. -f1)
        NEW_VERSION="$((MAJOR + 1)).0.0"
        ;;
    minor)
        MAJOR=$(echo "$CURRENT_VERSION" | cut -d. -f1)
        MINOR=$(echo "$CURRENT_VERSION" | cut -d. -f2)
        NEW_VERSION="$MAJOR.$((MINOR + 1)).0"
        ;;
    patch)
        MAJOR=$(echo "$CURRENT_VERSION" | cut -d. -f1)
        MINOR=$(echo "$CURRENT_VERSION" | cut -d. -f2)
        PATCH=$(echo "$CURRENT_VERSION" | cut -d. -f3)
        NEW_VERSION="$MAJOR.$MINOR.$((PATCH + 1))"
        ;;
    set)
        if ! [[ "$NEW_VERSION" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
            echo -e "${RED}Invalid version format: $NEW_VERSION${NC}"
            echo "Expected format: X.Y.Z (e.g., 1.0.0)"
            exit 1
        fi
        ;;
esac

echo -e "${BLUE}=====================================${NC}"
echo -e "${BLUE}  DIPAL Version Management${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""
echo -e "  Current: ${YELLOW}$CURRENT_VERSION${NC}"
echo -e "  New:     ${GREEN}$NEW_VERSION${NC}"
echo ""

if [ "$DRY_RUN" = true ]; then
    echo -e "${YELLOW}[DRY RUN] No files will be modified${NC}"
    echo ""
fi

set_version "$NEW_VERSION"

if [ "$CREATE_TAG" = true ] && [ "$DRY_RUN" = false ]; then
    echo ""
    echo -e "${YELLOW}Creating git tag v$NEW_VERSION...${NC}"
    git add -A
    git commit -m "chore: Bump version to $NEW_VERSION"
    git tag -a "v$NEW_VERSION" -m "Release v$NEW_VERSION"
    echo -e "${GREEN}Created tag: v$NEW_VERSION${NC}"
    echo -e "${YELLOW}Don't forget to push: git push origin develop --tags${NC}"
fi

echo ""
echo -e "${GREEN}Version updated successfully!${NC}"
echo -e "${BLUE}=====================================${NC}"
