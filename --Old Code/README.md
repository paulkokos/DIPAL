# DIPAL

Digital Image Processing and Analysis Library

## Description

A lightweight, simple, easy to use/understand, C++ image processing and analysis library. 
Built with modern C++ (C++23) techniques and designed to be independent of external libraries.

## Features

- Image loading and saving
- Image filtering and transformations
- Color space conversions
- Modern C++ design

## Installation

### Prerequisites

- C++23 compatible compiler (GCC 11+, Clang 14+, MSVC 19.30+)
- CMake 3.20 or higher

### Building from Source

```bash
git clone https://github.com/yourusername/DIPAL.git
cd DIPAL
mkdir build && cd build
cmake ..
cmake --build .
```

## Development with AstroNvim

This project includes configuration for AstroNvim:

1. Run the LSP setup script to generate compile_commands.json:
   ```bash
   ./tools/scripts/update_compile_commands.sh
   ```

2. Open the project in Neovim:
   ```bash
   nvim .
   ```

## Usage

```cpp
#include <DIPAL/DIPAL.hpp>

int main() {
    // TODO: Add usage example
    return 0;
}
```

## Documentation

See the [documentation](docs/README.md) for detailed API reference and examples.

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

## License

This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE) file for details.
