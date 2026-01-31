# DIPAL Scripts

Developer scripts for building, testing, and maintaining the DIPAL library.

## Quick Reference

| Script | Description | Common Usage |
|--------|-------------|--------------|
| `build.sh` | Build the project | `./scripts/build.sh` |
| `test.sh` | Run tests | `./scripts/test.sh` |
| `clean.sh` | Clean build directories | `./scripts/clean.sh` |
| `format.sh` | Format source code | `./scripts/format.sh` |
| `lint.sh` | Run clang-tidy | `./scripts/lint.sh` |
| `configure.sh` | Configure CMake only | `./scripts/configure.sh` |
| `install.sh` | Install the library | `./scripts/install.sh -p ~/local` |
| `coverage.sh` | Generate coverage report | `./scripts/coverage.sh -o` |

## Scripts

### build.sh

Build the project with various options.

```bash
./scripts/build.sh              # Release build
./scripts/build.sh -d           # Debug build
./scripts/build.sh -c           # Clean build (removes build dir first)
./scripts/build.sh -j 8         # Use 8 parallel jobs
./scripts/build.sh --no-tests   # Skip building tests
./scripts/build.sh --install --prefix ~/local
```

### test.sh

Run the test suite.

```bash
./scripts/test.sh               # Run all tests
./scripts/test.sh -v            # Verbose output
./scripts/test.sh -b            # Build first, then test
./scripts/test.sh -l            # List available tests
./scripts/test.sh "Image*"      # Run tests matching pattern
```

### clean.sh

Clean build artifacts.

```bash
./scripts/clean.sh              # Clean build/ and build-debug/
./scripts/clean.sh -a           # Clean all (including cmake-build-*, etc.)
./scripts/clean.sh -n           # Dry run (show what would be deleted)
```

### format.sh

Format source code using clang-format.

```bash
./scripts/format.sh             # Format all source files
./scripts/format.sh -c          # Check only (CI mode)
./scripts/format.sh -v          # Verbose (show files being processed)
```

### lint.sh

Run clang-tidy static analysis.

```bash
./scripts/lint.sh               # Lint all source files
./scripts/lint.sh -f            # Apply suggested fixes
./scripts/lint.sh src/Image/*.cpp  # Lint specific files
```

### configure.sh

Configure CMake without building. Useful for IDE integration.

```bash
./scripts/configure.sh          # Configure for Release
./scripts/configure.sh -d       # Configure for Debug
```

This generates `compile_commands.json` for IDE LSP support.

### install.sh

Install the library to a prefix.

```bash
./scripts/install.sh -p ~/local     # Install to ~/local
./scripts/install.sh -b -p ~/local  # Build and install
sudo ./scripts/install.sh           # Install to /usr/local
```

### coverage.sh

Generate code coverage reports.

```bash
./scripts/coverage.sh           # Generate coverage report
./scripts/coverage.sh -o        # Generate and open in browser
./scripts/coverage.sh -c        # Clean rebuild with coverage
```

Requires: `lcov` and `genhtml`

## Typical Workflows

### First-time setup

```bash
./scripts/configure.sh    # Generate compile_commands.json for IDE
./scripts/build.sh        # Build the project
./scripts/test.sh         # Run tests
```

### Development cycle

```bash
./scripts/build.sh && ./scripts/test.sh
# or
./scripts/test.sh -b      # Build and test in one command
```

### Before committing

```bash
./scripts/format.sh       # Format code
./scripts/lint.sh         # Check for issues
./scripts/test.sh         # Run tests
```

### CI/CD

```bash
./scripts/format.sh -c    # Check formatting (fail if not formatted)
./scripts/build.sh -c     # Clean build
./scripts/test.sh -v      # Run tests with verbose output
```

## Requirements

- CMake 3.24+
- Ninja (recommended) or Make
- C++23 compiler (GCC 13+, Clang 18+, MSVC 2022+)
- clang-format (for formatting)
- clang-tidy (for linting)
- lcov (for coverage)
