# Contributing to DIPAL

Thank you for your interest in contributing to DIPAL! This document provides guidelines and instructions for contributing.

## Code of Conduct

Please read and follow our [Code of Conduct](CODE_OF_CONDUCT.md).

## How to Contribute

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Pull Request Process

1. Ensure your code follows the project's coding style (run clang-format)
2. Update documentation if necessary
3. Add tests for new functionality
4. Make sure all tests pass
5. Wait for review and address any feedback

## Coding Standards

- Follow the Google C++ Style Guide with modifications as defined in .clang-format
- Use C++23 features appropriately
- Write clear, meaningful comments
- Include unit tests for all new functionality

## Development Setup

See the README.md for instructions on setting up the development environment.# Contributing to DIPAL

Thank you for your interest in contributing to DIPAL! This document provides guidelines and instructions for contributing.

## Code of Conduct

We expect all contributors to follow our Code of Conduct, which promotes a welcoming and inclusive environment.

## How to Contribute

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Pull Request Process

1. Ensure your code follows the project's coding style (run clang-format)
2. Update documentation if necessary
3. Add tests for new functionality
4. Make sure all tests pass
5. Wait for review and address any feedback

## Coding Standards

- Follow the Google C++ Style Guide with modifications as defined in `.clang-format`
- Use C++23 features appropriately
- Write clear, meaningful comments
- Include unit tests for all new functionality

## C++23 Guidelines

- Use `std::span` instead of pointer + size pairs
- Use `std::expected` for operations that can fail
- Use named parameters with designated initializers when appropriate
- Use concepts to constrain templates
- Use constexpr evaluation where possible

## Design Pattern Guidelines

- Prefer composition over inheritance
- Use the Strategy pattern for algorithms that can vary
- Use the Factory pattern for object creation
- Use the Command pattern for operations
- Use the Observer pattern for notifications

## Development Setup

### Prerequisites

- C++23 compatible compiler (GCC 12+, Clang 15+, MSVC 19.34+)
- CMake 3.20 or higher
- (Optional) Clang tools (clang-format, clang-tidy)

### Building the Library

```bash
# Clone the repository
git clone https://github.com/yourusername/DIPAL.git
cd DIPAL

# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build .

# Run tests
ctest

# Install (optional)
cmake --install .
