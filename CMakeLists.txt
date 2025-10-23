cmake_minimum_required(VERSION 3.10)
project(UARTSimulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Main library
add_library(uart_driver STATIC
    src/uart_driver.cpp
    src/uart_registers.cpp
)

target_include_directories(uart_driver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Main executable
add_executable(uart_demo
    src/main.cpp
)

target_link_libraries(uart_demo PRIVATE uart_driver)

# Test executable
add_executable(uart_tests
    tests/test_main.cpp
    tests/uart_basic_tests.cpp
    tests/uart_fifo_tests.cpp
)

target_link_libraries(uart_tests PRIVATE uart_driver)

# Enable testing
enable_testing()
add_test(NAME UARTTests COMMAND uart_tests)
