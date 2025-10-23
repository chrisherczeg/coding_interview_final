#!/bin/bash

# Build script for Linux/macOS

echo "Building UART Driver Project..."
echo "==============================="

# Create build directory
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Run CMake
echo "Running CMake..."
cmake ..

if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    exit 1
fi

# Build
echo "Building..."
make

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo ""
echo "Build successful!"
echo ""
echo "To run tests:  ./build/uart_tests"
echo "To run demo:   ./build/uart_demo"
