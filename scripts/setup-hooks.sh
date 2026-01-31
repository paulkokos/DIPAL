#!/bin/bash
# DIPAL Git Hooks Setup Script
# Usage: ./scripts/setup-hooks.sh

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

echo -e "${BLUE}=====================================${NC}"
echo -e "${BLUE}  DIPAL Git Hooks Setup${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""

cd "$PROJECT_ROOT"

# Check if pre-commit is available
if command -v pre-commit &> /dev/null; then
    echo -e "${YELLOW}Installing pre-commit hooks...${NC}"
    pre-commit install
    pre-commit install --hook-type commit-msg
    echo -e "${GREEN}pre-commit hooks installed!${NC}"
    echo ""
    echo "To run on all files: pre-commit run --all-files"
else
    echo -e "${YELLOW}pre-commit not found. Installing manual git hooks...${NC}"

    # Create hooks directory
    mkdir -p .git/hooks

    # Create pre-commit hook
    cat > .git/hooks/pre-commit << 'EOF'
#!/bin/bash
# DIPAL Pre-commit Hook

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}Running pre-commit checks...${NC}"

# Get staged C++ files
STAGED_CPP=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(cpp|hpp|h|c)$' || true)

if [ -n "$STAGED_CPP" ]; then
    # Check formatting
    echo "Checking formatting..."
    UNFORMATTED=""
    for file in $STAGED_CPP; do
        if [ -f "$file" ]; then
            if ! clang-format --dry-run --Werror "$file" 2>/dev/null; then
                UNFORMATTED="$UNFORMATTED $file"
            fi
        fi
    done

    if [ -n "$UNFORMATTED" ]; then
        echo -e "${RED}The following files need formatting:${NC}"
        echo "$UNFORMATTED"
        echo ""
        echo "Run: ./scripts/format.sh"
        exit 1
    fi
fi

echo -e "${GREEN}All checks passed!${NC}"
exit 0
EOF

    chmod +x .git/hooks/pre-commit
    echo -e "${GREEN}Manual pre-commit hook installed!${NC}"
fi

echo ""
echo -e "${BLUE}=====================================${NC}"
echo ""
echo "Hooks installed. They will run automatically on git commit."
echo ""
echo "To skip hooks (not recommended): git commit --no-verify"
echo ""
echo -e "${BLUE}=====================================${NC}"
