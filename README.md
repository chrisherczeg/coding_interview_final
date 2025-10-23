# UART Driver - Firmware Engineer Interview Challenge

## Overview

This project contains a simulated UART (Universal Asynchronous Receiver/Transmitter) hardware driver written in C++. It models a typical embedded systems peripheral with memory-mapped registers and FIFO buffers.

**Your Task**: The codebase contains a bug that causes test failures. Your job is to:
1. Build the project
2. Run the tests to identify which tests are failing
3. Debug and locate the root cause of the bug
4. Fix the bug
5. Verify all tests pass

## Project Structure

```
.
├── CMakeLists.txt          # Build configuration
├── include/
│   ├── uart_driver.h       # Main UART driver interface
│   └── uart_registers.h    # Hardware register definitions
├── src/
│   ├── uart_driver.cpp     # UART driver implementation
│   ├── uart_registers.cpp  # Register access implementation
│   └── main.cpp            # Demo application
└── tests/
    ├── test_main.cpp       # Test runner
    ├── uart_basic_tests.cpp    # Basic functionality tests
    └── uart_fifo_tests.cpp     # FIFO boundary tests
```

## Building the Project

### Prerequisites
- CMake 3.10 or higher
- C++11 compatible compiler:
  - **Windows**: Visual Studio 2015+ or MinGW
  - **Linux**: GCC 4.8+ or Clang 3.3+

### Windows (Visual Studio)

```powershell
mkdir build
cd build
cmake ..
cmake --build .
```

Alternatively, open the generated `.sln` file in Visual Studio.

### Windows (MinGW)

```powershell
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```

### Linux

```bash
mkdir build
cd build
cmake ..
make
```

## Running the Tests

### Windows
```powershell
cd build
.\Debug\uart_tests.exe    # Visual Studio
# or
.\uart_tests.exe          # MinGW
```

### Linux
```bash
cd build
./uart_tests
```

## Running the Demo

### Windows
```powershell
cd build
.\Debug\uart_demo.exe    # Visual Studio
# or
.\uart_demo.exe          # MinGW
```

### Linux
```bash
cd build
./uart_demo
```

## What the Code Does

This project simulates a UART peripheral similar to what you'd find in microcontrollers like ARM Cortex-M, AVR, or PIC devices. The implementation includes:

- **Hardware Registers**: Control, Status, Data, and Baud Rate registers
- **TX/RX FIFOs**: 16-byte deep transmission and reception buffers
- **Status Flags**: Empty, Full, Overrun, and Frame Error detection
- **Configuration**: Baud rate, parity (even/odd), enable/disable controls

### Key Components

1. **UARTRegisters**: Models memory-mapped hardware registers
2. **UARTDriver**: Provides the driver interface and manages FIFOs
3. **Tests**: Validate functionality including boundary conditions

## Hints

- Pay attention to test failures, especially in the FIFO tests
- Consider how circular buffers work and their capacity constraints
- Think about off-by-one errors
- Use a debugger to step through failing test cases
- The bug is subtle but realistic - it's the kind of issue that could appear in production firmware

## Expected Behavior

When working correctly:
- The FIFO should hold exactly 16 bytes
- Writing to a full FIFO should fail gracefully
- Reading from an empty FIFO should fail gracefully
- All tests should pass

## Debugging Tips

1. Run the tests and note which ones fail
2. Add debug output to understand FIFO state
3. Check the FIFO management functions carefully
4. Verify boundary conditions (empty, full, wrap-around)
5. Use assertions or print statements to validate assumptions

Good luck!
