#include "uart_driver.h"
#include <iostream>
#include <cstring>

namespace uart {
namespace test {

extern int tests_run;
extern int tests_passed;
extern int tests_failed;
extern void reportTest(const char* name, bool passed);

#define TEST(name, condition) \
    reportTest(name, (condition))

void testFifoNearFull() {
    std::cout << "\n=== FIFO Near-Full Tests ===" << std::endl;
    
    UARTDriver uart;
    uart.initialize(115200);
    
    // Write 15 bytes (FIFO_DEPTH - 1)
    uint8_t data[20];
    for (int i = 0; i < 20; i++) {
        data[i] = i;
    }
    
    size_t written = uart.writeData(data, 15);
    TEST("Write 15 bytes to TX FIFO", written == 15);
    TEST("TX FIFO count is 15", uart.getTxFifoCount() == 15);
    TEST("Can still transmit at 15 bytes", uart.canTransmit());
    
    // Try to write one more byte (should work, FIFO depth is 16)
    bool write_ok = uart.writeByte(0xFF);
    TEST("Write 16th byte succeeds", write_ok);
    TEST("TX FIFO count is 16", uart.getTxFifoCount() == 16);
    
    // At this point FIFO should be full
    TEST("Cannot transmit when full", !uart.canTransmit());
    
    // Try to write another byte (should fail)
    bool write_fail = uart.writeByte(0xEE);
    TEST("Write to full FIFO fails", !write_fail);
    TEST("TX FIFO count still 16", uart.getTxFifoCount() == 16);
}

void testFifoExactlyFull() {
    std::cout << "\n=== FIFO Exactly Full Tests ===" << std::endl;
    
    UARTDriver uart;
    uart.initialize(115200);
    
    // Write exactly FIFO_DEPTH bytes
    uint8_t data[16];
    for (int i = 0; i < 16; i++) {
        data[i] = i * 2;
    }
    
    size_t written = uart.writeData(data, 16);
    TEST("Write exactly 16 bytes", written == 16);
    TEST("TX FIFO count is 16", uart.getTxFifoCount() == 16);
    TEST("Cannot transmit when exactly full", !uart.canTransmit());
}

void testRxFifoFull() {
    std::cout << "\n=== RX FIFO Full Tests ===" << std::endl;
    
    UARTDriver uart;
    uart.initialize(115200);
    
    // Simulate receiving exactly FIFO_DEPTH bytes
    uint8_t rx_data[20];
    for (int i = 0; i < 20; i++) {
        rx_data[i] = 0x80 + i;
    }
    
    uart.simulateReceive(rx_data, 16);
    TEST("RX FIFO count is 16", uart.getRxFifoCount() == 16);
    TEST("Has data when full", uart.hasData());
    
    // Try to receive one more byte - should trigger overrun
    uart.simulateReceive(rx_data + 16, 1);
    TEST("Overrun error after exceeding FIFO", uart.hasError());
    
    // Clear the error
    uart.clearErrors();
    TEST("Error cleared", !uart.hasError());
}

void testFifoWrapAround() {
    std::cout << "\n=== FIFO Wrap-Around Tests ===" << std::endl;
    
    UARTDriver uart;
    uart.initialize(115200);
    
    // Fill FIFO partially
    uint8_t data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uart.writeData(data, 10);
    
    // Simulate transmitting some bytes
    uart.simulateTransmit(5);
    TEST("TX FIFO count is 5 after partial transmit", uart.getTxFifoCount() == 5);
    
    // Write more data to cause wrap-around
    uint8_t more_data[10] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    size_t written = uart.writeData(more_data, 10);
    
    // Should be able to write (16-5) = 11 bytes, but we're only writing 10
    TEST("Write 10 more bytes", written == 10);
    TEST("TX FIFO count is 15", uart.getTxFifoCount() == 15);
}

void testAlternatingReadWrite() {
    std::cout << "\n=== Alternating Read/Write Tests ===" << std::endl;
    
    UARTDriver uart;
    uart.initialize(115200);
    
    // Alternately write and read to stress the FIFO
    for (int i = 0; i < 5; i++) {
        uint8_t tx_byte = 0x10 + i;
        TEST("Write byte in loop", uart.writeByte(tx_byte));
        uart.simulateTransmit(1);
        
        uint8_t rx_byte = 0x20 + i;
        uart.simulateReceive(&rx_byte, 1);
        
        uint8_t read_byte = 0;
        TEST("Read byte in loop", uart.readByte(read_byte));
        TEST("Read data matches", read_byte == rx_byte);
    }
}

void testBoundaryConditions() {
    std::cout << "\n=== Boundary Condition Tests ===" << std::endl;
    
    UARTDriver uart;
    uart.initialize(115200);
    
    // Test writing 0 bytes
    uint8_t dummy[1] = {0};
    size_t written = uart.writeData(dummy, 0);
    TEST("Write 0 bytes", written == 0);
    
    // Test reading from empty FIFO
    uint8_t buffer[10];
    size_t read = uart.readData(buffer, 10);
    TEST("Read from empty FIFO returns 0", read == 0);
    
    // Test null pointer handling
    written = uart.writeData(nullptr, 5);
    TEST("Write with null pointer returns 0", written == 0);
    
    read = uart.readData(nullptr, 5);
    TEST("Read with null pointer returns 0", read == 0);
}

void testSequentialFill() {
    std::cout << "\n=== Sequential Fill Test ===" << std::endl;
    
    UARTDriver uart;
    uart.initialize(115200);
    
    // Write bytes one at a time until full
    int bytes_written = 0;
    for (int i = 0; i < 20; i++) {
        if (uart.writeByte(i)) {
            bytes_written++;
        } else {
            break;
        }
    }
    
    TEST("Wrote exactly 16 bytes", bytes_written == 16);
    TEST("TX FIFO count matches", uart.getTxFifoCount() == 16);
    
    // Now drain and refill
    uart.simulateTransmit(16);
    TEST("FIFO empty after transmit", uart.getTxFifoCount() == 0);
    
    bytes_written = 0;
    for (int i = 0; i < 20; i++) {
        if (uart.writeByte(100 + i)) {
            bytes_written++;
        } else {
            break;
        }
    }
    
    TEST("Wrote exactly 16 bytes again", bytes_written == 16);
}

int runFifoTests() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "Running FIFO Boundary Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    
    testFifoNearFull();
    testFifoExactlyFull();
    testRxFifoFull();
    testFifoWrapAround();
    testAlternatingReadWrite();
    testBoundaryConditions();
    testSequentialFill();
    
    return tests_failed;
}

} // namespace test
} // namespace uart
