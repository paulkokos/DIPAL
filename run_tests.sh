#!/bin/bash

# Set the path to the build directory (where CMake output is)
BUILD_DIR="build"

# Change directory to the build directory
cd "$BUILD_DIR" || {
  echo "Failed to change directory to $BUILD_DIR"
  exit 1
}

# Run the tests (assuming your test executable is named binary_image_tests)
echo "Running tests..."
ctest -j4
# ./binary_image_tests

# Check the test results
TEST_RESULT=$?

if [ $TEST_RESULT -eq 0 ]; then
  echo "All tests passed."
  exit 0
else
  echo "Some tests failed."
  exit 1
fi
