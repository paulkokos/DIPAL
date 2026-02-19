# DIPAL Documentation

This directory contains documentation for the DIPAL (Digital Image Processing and Analysis Library).

## Structure

- `api/`: API documentation generated from code
- `examples/`: Example usage and tutorials

## Building Documentation

To generate API documentation, run:

```bash
# Generate documentation with Doxygen
./scripts/docs.sh

# Generate and open in browser
./scripts/docs.sh --open

# Clean and regenerate
./scripts/docs.sh --clean --open
```

### Requirements

- **doxygen**: Required for generating API documentation
- **graphviz**: Optional, for generating call graphs and diagrams

### Installation

**Ubuntu/Debian:**
```bash
sudo apt install doxygen graphviz
```

**macOS:**
```bash
brew install doxygen graphviz
```

### Output

The generated documentation is placed in `docs/api/html/` and can be viewed by opening `docs/api/html/index.html` in a web browser.
