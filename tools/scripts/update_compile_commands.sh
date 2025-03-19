#!/bin/bash
# Generate compile_commands.json for LSP

BUILD_DIR=${1:-build}

if [ ! -d "$BUILD_DIR" ]; then
  mkdir -p "$BUILD_DIR"
  cd "$BUILD_DIR"
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
else
  cd "$BUILD_DIR"
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
fi

# Link compilation database to project root for LSP
cd ../../
ln -sf "$BUILD_DIR/compile_commands.json" .
