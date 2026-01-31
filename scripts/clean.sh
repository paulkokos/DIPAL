#!/bin/bash
# DIPAL Clean Script
# Usage: ./scripts/clean.sh [options]

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

ALL=false
DRY_RUN=false

usage() {
    echo "DIPAL Clean Script"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -a, --all       Clean all build directories and generated files"
    echo "  -n, --dry-run   Show what would be deleted without deleting"
    echo "  -h, --help      Show this help message"
    echo ""
    echo "Directories cleaned by default:"
    echo "  - build/"
    echo "  - build-debug/"
    echo ""
    echo "Additional with --all:"
    echo "  - cmake-build-*/"
    echo "  - compile_commands.json (symlink)"
    echo "  - install/"
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -a|--all)
            ALL=true
            shift
            ;;
        -n|--dry-run)
            DRY_RUN=true
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
echo -e "${BLUE}  DIPAL Clean${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""

cd "$PROJECT_ROOT"

# Function to remove directory/file
remove_item() {
    local item="$1"
    if [ -e "$item" ] || [ -L "$item" ]; then
        if [ "$DRY_RUN" = true ]; then
            echo -e "${YELLOW}Would remove: ${NC}$item"
        else
            echo -e "${YELLOW}Removing: ${NC}$item"
            rm -rf "$item"
        fi
    fi
}

# Standard build directories
remove_item "build"
remove_item "build-debug"

# Additional cleanup with --all
if [ "$ALL" = true ]; then
    # IDE build directories
    for dir in cmake-build-*; do
        [ -e "$dir" ] && remove_item "$dir"
    done

    # Generated files
    remove_item "compile_commands.json"
    remove_item "install"

    # Testing artifacts
    remove_item "Testing"
fi

echo ""
if [ "$DRY_RUN" = true ]; then
    echo -e "${YELLOW}Dry run complete. No files were deleted.${NC}"
else
    echo -e "${GREEN}Clean complete!${NC}"
fi
echo -e "${BLUE}=====================================${NC}"
