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
| `cppcheck.sh` | Run cppcheck analysis | `./scripts/cppcheck.sh` |
| `sanitize.sh` | Build with sanitizers | `./scripts/sanitize.sh` |
| `valgrind.sh` | Memory analysis | `./scripts/valgrind.sh` |
| `coverage.sh` | Code coverage report | `./scripts/coverage.sh` |
| `benchmark.sh` | Performance benchmarks | `./scripts/benchmark.sh` |
| `docs.sh` | Generate documentation | `./scripts/docs.sh` |
| `package.sh` | Create release packages | `./scripts/package.sh` |
| `version.sh` | Manage versions | `./scripts/version.sh` |
| `configure.sh` | Configure CMake only | `./scripts/configure.sh` |
| `install.sh` | Install the library | `./scripts/install.sh` |
| `setup-hooks.sh` | Install git hooks | `./scripts/setup-hooks.sh` |

## Build Scripts

### build.sh

Build the project with various options.

```bash
./scripts/build.sh              # Release build
./scripts/build.sh -d           # Debug build
./scripts/build.sh -c           # Clean build
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
./scripts/clean.sh -a           # Clean all
./scripts/clean.sh -n           # Dry run
```

### configure.sh

Configure CMake without building. Generates `compile_commands.json` for IDE support.

```bash
./scripts/configure.sh          # Configure for Release
./scripts/configure.sh -d       # Configure for Debug
```

### install.sh

Install the library.

```bash
./scripts/install.sh -p ~/local     # Install to ~/local
./scripts/install.sh -b -p ~/local  # Build and install
sudo ./scripts/install.sh           # Install to /usr/local
```

## Code Quality Scripts

### format.sh

Format source code using clang-format.

```bash
./scripts/format.sh             # Format all source files
./scripts/format.sh -c          # Check only (CI mode)
./scripts/format.sh -v          # Verbose
```

### lint.sh

Run clang-tidy static analysis.

```bash
./scripts/lint.sh               # Lint all source files
./scripts/lint.sh -f            # Apply suggested fixes
./scripts/lint.sh src/Image/*.cpp
```

### cppcheck.sh

Run cppcheck static analysis.

```bash
./scripts/cppcheck.sh           # Default analysis
./scripts/cppcheck.sh -s error  # Only errors
./scripts/cppcheck.sh --html    # Generate HTML report
```

### sanitize.sh

Build with runtime sanitizers.

```bash
./scripts/sanitize.sh           # AddressSanitizer (default)
./scripts/sanitize.sh -u        # UndefinedBehaviorSanitizer
./scripts/sanitize.sh -t        # ThreadSanitizer
./scripts/sanitize.sh --all     # Run all sanitizers
```

### valgrind.sh

Memory analysis with Valgrind.

```bash
./scripts/valgrind.sh           # Memory check
./scripts/valgrind.sh -c        # Cache profiling
./scripts/valgrind.sh -l        # Call profiling
./scripts/valgrind.sh -e        # Thread errors
```

### coverage.sh

Generate code coverage reports.

```bash
./scripts/coverage.sh           # Generate report
./scripts/coverage.sh -o        # Open in browser
./scripts/coverage.sh -c        # Clean rebuild
```

Requires: `lcov` and `genhtml`

## Performance Scripts

### benchmark.sh

Run performance benchmarks.

```bash
./scripts/benchmark.sh              # Run benchmarks
./scripts/benchmark.sh -i 10        # 10 iterations
./scripts/benchmark.sh --save-baseline
./scripts/benchmark.sh -b           # Compare to baseline
```

## Documentation & Release

### docs.sh

Generate API documentation using Doxygen.

```bash
./scripts/docs.sh               # Generate docs
./scripts/docs.sh -o            # Open in browser
./scripts/docs.sh -c            # Clean and regenerate
```

Requires: `doxygen`, optionally `graphviz`

### package.sh

Create release packages.

```bash
./scripts/package.sh            # Create binary package
./scripts/package.sh -s         # Include source archive
./scripts/package.sh -v 1.0.0   # Override version
```

### version.sh

Manage semantic versioning.

```bash
./scripts/version.sh show       # Show current version
./scripts/version.sh patch      # 0.1.4 -> 0.1.5
./scripts/version.sh minor      # 0.1.4 -> 0.2.0
./scripts/version.sh major      # 0.1.4 -> 1.0.0
./scripts/version.sh set 1.0.0  # Set specific version
./scripts/version.sh patch --tag  # Bump and create git tag
```

## Setup Scripts

### setup-hooks.sh

Install git pre-commit hooks.

```bash
./scripts/setup-hooks.sh        # Install hooks
```

Uses `pre-commit` if available, otherwise installs manual hooks.

## Typical Workflows

### First-time setup

```bash
./scripts/setup-hooks.sh      # Install git hooks
./scripts/configure.sh        # Generate compile_commands.json
./scripts/build.sh            # Build the project
./scripts/test.sh             # Run tests
```

### Development cycle

```bash
./scripts/test.sh -b          # Build and test
```

### Before committing

```bash
./scripts/format.sh           # Format code
./scripts/lint.sh             # Check for issues
./scripts/test.sh             # Run tests
```

### Full quality check

```bash
./scripts/format.sh -c        # Check formatting
./scripts/lint.sh             # clang-tidy
./scripts/cppcheck.sh         # cppcheck
./scripts/sanitize.sh --all   # All sanitizers
./scripts/test.sh             # Tests
```

### Creating a release

```bash
./scripts/version.sh patch --tag
./scripts/package.sh -s
git push origin develop --tags
```

## Requirements

| Tool | Required For | Install (Ubuntu) |
|------|--------------|------------------|
| CMake 3.24+ | Building | `apt install cmake` |
| Ninja | Building | `apt install ninja-build` |
| GCC 13+ / Clang 18+ | Building | PPA / llvm.org |
| clang-format | Formatting | `apt install clang-format` |
| clang-tidy | Linting | `apt install clang-tidy` |
| cppcheck | Static analysis | `apt install cppcheck` |
| lcov | Coverage | `apt install lcov` |
| valgrind | Memory analysis | `apt install valgrind` |
| doxygen | Documentation | `apt install doxygen graphviz` |
| pre-commit | Git hooks | `pip install pre-commit` |
